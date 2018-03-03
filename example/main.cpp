#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/file_mutex.hpp>

#include <thread>
#include <iostream>
#include <mutex>

int main(int argc, char* argv[])
{
    const auto path = "test_file.txt";
    std::ofstream file(path, std::ios::app);
    boost::interprocess::file_mutex guard(path);

    std::lock_guard<boost::interprocess::file_mutex> lock(guard);

    for (auto i = 0u; i < 10; ++i)
    {
        std::cout << "Writing " << i << " " << argv[1] << "\n";
        file << argv[1] << "\n";
        std::this_thread::sleep_for(std::chrono::seconds{ 1 });
    }
}