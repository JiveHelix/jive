#include <jive/thread_pool.h>
#include <cmath>


namespace jive
{


namespace detail
{


Sentry_::Sentry_()
    :
    mutex_{},
    isDone_(false),
    inProgress_(false),
    condition_{},
    exceptionPtr_{}
{

}


void Sentry_::Reset()
{
    std::lock_guard lock(this->mutex_);
    this->inProgress_ = false;
    this->isDone_ = false;
    this->exceptionPtr_ = std::exception_ptr{};
}


void Sentry_::Signal(std::optional<std::exception_ptr> exceptionPtr)
{
    std::lock_guard lock(this->mutex_);

    if (exceptionPtr)
    {
        this->exceptionPtr_ = *exceptionPtr;
    }

    this->inProgress_ = false;
    this->isDone_ = true;
    this->condition_.notify_one();
}


void Sentry_::Wait()
{
    std::unique_lock lock(this->mutex_);

    if (!this->isDone_)
    {
        this->condition_.wait(
            lock,
            [this]() -> bool
            {
                return this->isDone_;
            });
    }

    if (this->exceptionPtr_)
    {
        std::rethrow_exception(this->exceptionPtr_);
    }
}


bool Sentry_::InProgress() const
{
    std::lock_guard lock(this->mutex_);

    return this->inProgress_;
}


bool Sentry_::IsDone() const
{
    std::lock_guard lock(this->mutex_);

    return this->isDone_;
}


bool Sentry_::Claim()
{
    std::lock_guard lock(this->mutex_);

    if (!this->inProgress_ && !this->isDone_)
    {
        this->inProgress_ = true;

        return true;
    }

    return false;
}


Job::Job(const SharedSentry &sentry, const std::function<void()> &task)
    :
    sentry_(sentry),
    task_(task)
{

}


void Job::Run()
{
    if (!this->sentry_->Claim())
    {
        // Someone else already claimed this job.
        return;
    }

    try
    {
        this->task_();
    }
    catch (...)
    {
        this->sentry_->Signal(std::current_exception());

        return;
    }

    this->sentry_->Signal();
}


Queue::Queue()
    :
    mutex_{},
    isRunning_(true),
    jobsCondition_{},
    concurrency_(std::thread::hardware_concurrency()),
    sentryPool_{},
    jobs_{},
    activeCount_(0)
{
    // Initially allow queueing twice as many jobs as the hardware allows
    // to run concurrently.
    auto count = this->concurrency_ * 2;

    while (count--)
    {
        this->sentryPool_.push_back(std::make_shared<Sentry_>());
    }
}


SharedSentry Queue::AddJob(const std::function<void()> &job)
{
    std::lock_guard lock(this->mutex_);

    SharedSentry sentry;

    if (this->sentryPool_.empty())
    {
        sentry = std::make_shared<Sentry_>();
    }
    else
    {
        // Reuse the a sentry.
        sentry = this->sentryPool_.front();
        this->sentryPool_.pop_front();
        sentry->Reset();
    }

    this->jobs_.emplace_back(sentry, job);
    this->jobsCondition_.notify_all();

    return sentry;
}


void Queue::ReportJobDone()
{
    std::lock_guard lock(this->mutex_);

    this->activeCount_--;
}


std::optional<Job> Queue::RequestJob()
{
    std::unique_lock lock(this->mutex_);

    if (this->jobs_.empty())
    {
        this->jobsCondition_.wait(
            lock,
            [this]() -> bool
            {
                return (
                    !this->jobs_.empty()
                    || !this->isRunning_);
            });
    }

    if (!this->isRunning_)
    {
        return {};
    }

    auto job = this->jobs_.front();
    this->jobs_.pop_front();

    this->activeCount_++;

    return job;
}


bool Queue::IsRunning() const
{
    std::lock_guard lock(this->mutex_);
    return this->isRunning_;
}


void Queue::Stop()
{
    std::lock_guard lock(this->mutex_);
    this->isRunning_ = false;
    this->jobsCondition_.notify_all();
}


void Queue::Start()
{
    std::lock_guard lock(this->mutex_);
    this->isRunning_ = true;
}


void Queue::ReturnSentry(const SharedSentry &sharedSentry)
{
    std::lock_guard lock(this->mutex_);
    this->sentryPool_.push_back(sharedSentry);
}


size_t Queue::GetQueuedCount() const
{
    std::lock_guard lock(this->mutex_);

    return this->jobs_.size();
}


int64_t Queue::GetActiveCount() const
{
    std::lock_guard lock(this->mutex_);

    return this->activeCount_;
}


Thread::Thread()
    :
    queue_(),
    thread_()
{

}


Thread::Thread(const std::shared_ptr<Queue> &queue)
    :
    queue_(queue),
    thread_()
{

}


Thread::Thread(Thread &&other)
    :
    queue_(other.queue_),
    thread_(std::move(other.thread_))
{

}


Thread & Thread::operator=(Thread &&other)
{
    if (this->thread_.joinable())
    {
        this->thread_.join();
    }

    this->thread_ = std::move(other.thread_);

    return *this;
}


Thread::~Thread()
{
    this->Stop();
}


void Thread::Start()
{
    if (!this->thread_.joinable())
    {
        this->thread_ =
            std::thread(std::bind(&Thread::Run_, this));
    }
}


void Thread::Stop()
{
    if (this->thread_.joinable())
    {
        this->thread_.join();
    }
}


void Thread::Run_()
{
    while (this->queue_->IsRunning())
    {
        auto job = this->queue_->RequestJob();

        if (job)
        {
            job->Run();
            this->queue_->ReportJobDone();
        }
    }
}


} // end namespace detail


ThreadPool::ThreadPool()
    :
    mutex_(),
    queue_(std::make_shared<detail::Queue>()),
    threads_(),
    loadFactor_(1.0)
{
    auto count = std::thread::hardware_concurrency();
    this->threads_.reserve(count);

    while (count--)
    {
        this->threads_.emplace_back(this->queue_);
    }

    this->ResumeThreads_();
}


ThreadPool::~ThreadPool()
{
    this->PauseThreads_();
}


Sentry ThreadPool::AddJob(const std::function<void()> &job)
{
    return Sentry(
        this->queue_->AddJob(job),
        this->queue_);
}


size_t ThreadPool::GetConcurrency() const
{
    std::lock_guard lock(this->mutex_);

    return this->threads_.size();
}


size_t ThreadPool::GetQueuedCount() const
{
    return this->queue_->GetQueuedCount();
}


int64_t ThreadPool::GetActiveCount() const
{
    return this->queue_->GetActiveCount();
}


double ThreadPool::GetLoadFactor() const
{
    std::lock_guard lock(this->mutex_);

    return this->loadFactor_;
}


double ThreadPool::GetMinLoadFactor() const
{
    auto hardwareConcurrency =
        static_cast<double>(std::thread::hardware_concurrency());

    assert(hardwareConcurrency >= 1.0);

    // The minimum is to use 1 process.
    // If I allowed this to go to zero, there would be no threads in the pool.
    return 1.0 / hardwareConcurrency;
}


double ThreadPool::GetPressure() const
{
    auto queueCount = static_cast<double>(this->queue_->GetQueuedCount());

    std::lock_guard lock(this->mutex_);

    return queueCount / static_cast<double>(this->threads_.size());
}


void ThreadPool::SetLoadFactor(double loadFactor)
{
    auto hardwareConcurrency =
        static_cast<double>(std::thread::hardware_concurrency());

    auto minLoadFactor = this->GetMinLoadFactor();

    loadFactor = std::max(loadFactor, minLoadFactor);
    loadFactor = std::min(loadFactor, 1.0);

    auto count = static_cast<size_t>(
        std::round(loadFactor * hardwareConcurrency));

    count = std::min<size_t>(std::thread::hardware_concurrency(), count);

    std::lock_guard lock(this->mutex_);

    if (count > this->threads_.size())
    {
        this->PauseThreads_();

        this->threads_.reserve(count);
        size_t toInitialize = count - this->threads_.size();

        while (toInitialize--)
        {
            this->threads_.emplace_back(this->queue_);
        }

        this->ResumeThreads_();
    }
    else if (count < this->threads_.size())
    {
        this->PauseThreads_();
        this->threads_.resize(count);
        this->ResumeThreads_();
    }

    this->loadFactor_ =
        static_cast<double>(count)
        / static_cast<double>(std::thread::hardware_concurrency());
}


void ThreadPool::PauseThreads_()
{
    this->queue_->Stop();

    for (auto &thread: this->threads_)
    {
        thread.Stop();
    }
}

void ThreadPool::ResumeThreads_()
{
    this->queue_->Start();

    for (auto &thread: this->threads_)
    {
        thread.Start();
    }
}


std::shared_ptr<ThreadPool> threadPool;


std::shared_ptr<ThreadPool> GetThreadPool()
{
    if (!threadPool)
    {
        return threadPool = std::shared_ptr<ThreadPool>(new ThreadPool());
    }

    return threadPool;
}


} // end namespace jive
