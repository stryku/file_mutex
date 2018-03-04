# file_mutex

A `boost::interprocess::file_mutex` class proposal. Class' purpose is to synchronize file access from multiple processes. It has sharable and exclusive locking capabilities and can be used with scoped_lock and sharable_lock classes.

Example usage: 
```cpp
#include <boost/interprocess/sync/file_mutex.hpp>

#include <iostream>
#include <mutex>

int main(int argc, char* argv[])
{
    const auto path = "test_file.txt";
    std::ofstream file(path, std::ios::app);
    boost::interprocess::file_mutex file_mutex(path);

    std::lock_guard<boost::interprocess::file_mutex> lock(file_mutex);

    for (auto i = 0u; i < 10; ++i)
    {
        std::cout << "Writing " << i << " " << argv[1] << "\n";
        file << argv[1] << "\n";
        std::this_thread::sleep_for(std::chrono::seconds{ 1 });
    }
}
```
