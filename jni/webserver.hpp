#include <fost/file>


namespace fostlib {


    /// Start the web server over the specified root directory
    void start(const boost::filesystem::wpath &root);
    /// Wait for the web server to terminate
    void wait();
    /// Stop the web server
    void stop();


}
