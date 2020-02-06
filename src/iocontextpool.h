#pragma once

#include <thread>
#include <vector>
#include <boost/asio/io_context.hpp>

class IOContextPool {

    using work = boost::asio::io_context::work;

    size_t index;
    std::vector<boost::asio::io_context> io_contexts;
    std::vector<std::unique_ptr<work>> works;
    std::vector<std::thread> threads;

public:
    IOContextPool(size_t max_thread = std::thread::hardware_concurrency())
//    IOContextPool(size_t max_thread = 1)
        : index(0)
        , io_contexts(max_thread) {

        for (size_t i = 0 ; i < max_thread ; ++i) {
            works.push_back(std::unique_ptr<work>(new work(io_contexts[i])));
        }

        for (size_t i = 0 ; i < max_thread ; ++i) {
            threads.emplace_back([this, i](){
                io_contexts[i].run();
            });
        }
    }

    IOContextPool(const IOContextPool&) = delete;
    IOContextPool& operator=(const IOContextPool&) = delete;

    boost::asio::io_context& service() {
        return io_contexts[index++ % io_contexts.size()];
    }

    void stop() {
        for (std::unique_ptr<work>& work : works) {
            work.reset();
        }
        for (boost::asio::io_context& context : io_contexts) {
            context.stop();
        }
        for (std::thread& thread : threads) {

            if (thread.joinable())
                thread.join();
        }

        threads.clear();
        io_contexts.clear();
        works.clear();
    }
};
