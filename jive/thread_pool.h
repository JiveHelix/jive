#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <vector>
#include <deque>
#include <functional>
#include <exception>
#include <optional>
#include <cassert>


#ifdef AddJob
// Remove windows pollution!
#undef AddJob
#endif


namespace jive
{


namespace detail
{


class Sentry_
{
private:
    mutable std::mutex mutex_;
    bool isDone_;
    bool inProgress_;
    std::condition_variable condition_;
    std::exception_ptr exceptionPtr_;

public:
    Sentry_();

    Sentry_(const Sentry_ &) = delete;
    Sentry_(Sentry_ &&) = delete;
    Sentry_ & operator=(const Sentry_ &) = delete;
    Sentry_ & operator=(Sentry_ &&) = delete;

    void Reset();

    void Signal(std::optional<std::exception_ptr> exceptionPtr = std::nullopt);

    void Wait();

    bool InProgress() const;

    bool IsDone() const;

    bool Claim();
};


using SharedSentry = std::shared_ptr<Sentry_>;


class Job
{
public:
    Job(const SharedSentry &sentry, const std::function<void()> &task);

    void Run();

private:
    SharedSentry sentry_;
    std::function<void()> task_;
};


class Queue
{
public:
    Queue();

    SharedSentry AddJob(const std::function<void()> &job);

    std::optional<Job> RequestJob();

    void ReportJobDone();

    bool IsRunning() const;

    void Stop();

    void Start();

    void ReturnSentry(const SharedSentry &sharedSentry);

    size_t GetQueuedCount() const;

    int64_t GetActiveCount() const;

private:
    mutable std::mutex mutex_;
    bool isRunning_;
    std::condition_variable jobsCondition_;
    size_t concurrency_;
    std::deque<SharedSentry> sentryPool_;
    std::deque<Job> jobs_;
    int64_t activeCount_;
};


class Thread
{
public:
    Thread();

    Thread(const std::shared_ptr<Queue> &queue);

    Thread(const Thread &) = delete;

    Thread(Thread &&other);

    Thread & operator=(const Thread &) = delete;

    Thread & operator=(Thread &&other);

    ~Thread();

    void Start();

    void Stop();

private:
    void Run_();

private:
    std::shared_ptr<Queue> queue_;
    std::thread thread_;
};


} // end namespace detail


class Sentry
{
public:
    ~Sentry()
    {
        this->ReturnSentry_();
    }

    void Wait()
    {
        assert(this->sentry_);

        this->sentry_->Wait();
    }

    Sentry(const Sentry &) = delete;

    Sentry(Sentry &&other)
        :
        sentry_(std::move(other.sentry_)),
        queue_(std::move(other.queue_))
    {

    }

    Sentry & operator=(const Sentry &) = delete;

    Sentry & operator=(Sentry &&other)
    {
        this->ReturnSentry_();

        this->sentry_ = std::move(other.sentry_);
        this->queue_ = std::move(other.queue_);

        return *this;
    }

private:
    friend class ThreadPool;

    Sentry(
        const std::shared_ptr<detail::Sentry_> sentry,
        const std::shared_ptr<detail::Queue> &queue)
        :
        sentry_(sentry),
        queue_(queue)
    {

    }

    void ReturnSentry_()
    {
        if (this->sentry_)
        {
            assert(this->queue_);
            this->queue_->ReturnSentry(this->sentry_);
            this->sentry_.reset();
        }
    }

private:
    std::shared_ptr<detail::Sentry_> sentry_;
    std::shared_ptr<detail::Queue> queue_;
};


class ThreadPool
{
private:
    mutable std::mutex mutex_;
    std::shared_ptr<detail::Queue> queue_;
    std::vector<detail::Thread> threads_;
    double loadFactor_;

private:
    ThreadPool();

    void PauseThreads_();
    void ResumeThreads_();

public:
    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool & operator=(const ThreadPool &) = delete;
    ThreadPool & operator=(ThreadPool &&) = delete;

    Sentry AddJob(const std::function<void()> &job);

    size_t GetConcurrency() const;

    size_t GetQueuedCount() const;

    int64_t GetActiveCount() const;

    double GetLoadFactor() const;

    void SetLoadFactor(double loadFactor);

    double GetMinLoadFactor() const;

    double GetPressure() const;

    friend std::shared_ptr<ThreadPool> GetThreadPool();
};


std::shared_ptr<ThreadPool> GetThreadPool();


} // end namespace jive
