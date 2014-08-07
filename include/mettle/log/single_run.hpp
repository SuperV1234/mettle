#ifndef INC_METTLE_LOG_SINGLE_RUN_HPP
#define INC_METTLE_LOG_SINGLE_RUN_HPP

#include <ostream>

#include "core.hpp"
#include "term.hpp"
#include "verbose.hpp"

namespace mettle {

namespace log {

  class single_run : public test_logger {
  public:
    single_run(verbose vlog) : vlog_(vlog) {}

    void started_run() {
      vlog_.started_run();
    }

    void ended_run() {
      vlog_.ended_run();
    }

    void started_suite(const std::vector<std::string> &suites) {
      vlog_.started_suite(suites);
    }

    void ended_suite(const std::vector<std::string> &suites) {
      vlog_.ended_suite(suites);
    }

    void started_test(const test_name &test) {
      total_++;
      vlog_.started_test(test);
    }

    void passed_test(const test_name &test, const test_output &output) {
      passes_++;
      vlog_.passed_test(test, output);
    }

    void failed_test(const test_name &test, const std::string &message,
                     const test_output &output) {
      failures_.push_back({test, message});
      vlog_.failed_test(test, message, output);
    }

    void skipped_test(const test_name &test) {
      skips_++;
      vlog_.skipped_test(test);
    }

    void summarize() {
      using namespace term;

      if(vlog_.verbosity())
        vlog_.out << std::endl;

      vlog_.out << format(sgr::bold) << passes_ << "/" << total_
                << " tests passed";
      if(skips_)
        vlog_.out << " (" << skips_ << " skipped)";
      vlog_.out << reset() << std::endl;

      for(const auto &i : failures_) {
        vlog_.out << "  " << i.test.full_name() << " "
                  << format(sgr::bold, fg(color::red)) << "FAILED" << reset()
                  << ": " << i.message << std::endl;
      }
    }

    bool good() const {
      return failures_.empty();
    }
  private:
    struct failure {
      test_name test;
      std::string message;
    };

    verbose vlog_;
    size_t total_ = 0, passes_ = 0, skips_ = 0;
    std::vector<const failure> failures_;
  };

}

} // namespace mettle

#endif
