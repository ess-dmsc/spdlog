#pragma once

#ifndef SPDLOG_H
#error "spdlog.h must be included before this file."
#endif

#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/base_sink.h"

#include <graylog_logger/FileInterface.hpp>
#include <graylog_logger/GraylogInterface.hpp>
#include <graylog_logger/Log.hpp>

#include <map>
#include <mutex>

namespace spdlog {
namespace sinks {

template <typename Mutex>
class graylog_sink final : public spdlog::sinks::base_sink<Mutex> {
public:
  explicit graylog_sink(const spdlog::level::level_enum &LoggingLevel, const std::string &Host, int Port) {
    Log::RemoveAllHandlers(); // Remove the default (console) log handler
    Log::AddLogHandler(new Log::GraylogInterface(Host, Port));
    Log::SetMinimumSeverity(LogLevels[LoggingLevel]);
  }

private:
  std::map<int, Log::Severity> LogLevels{
      {SPDLOG_LEVEL_TRACE, Log::Severity::Debug},
      {SPDLOG_LEVEL_DEBUG, Log::Severity::Info},
      {SPDLOG_LEVEL_INFO, Log::Severity::Notice},
      {SPDLOG_LEVEL_WARN, Log::Severity::Warning},
      {SPDLOG_LEVEL_ERROR, Log::Severity::Error},
      {SPDLOG_LEVEL_CRITICAL, Log::Severity::Critical},
      {SPDLOG_LEVEL_OFF, Log::Severity::Alert}};

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    Log::Msg(LogLevels[msg.level],  std::string(msg.payload.data()));
  }
  void flush_() override{};
};
using graylog_sink_mt = graylog_sink<std::mutex>;
using graylog_sink_st = graylog_sink<details::null_mutex>;
}

//
// factory functions
//
template<typename Factory = default_factory>
inline std::shared_ptr<logger> graylog_mt(const std::string &logger_name, const spdlog::level::level_enum &LoggingLevel, const std::string &host, const int &port)
{
  return Factory::template create<sinks::graylog_sink_mt>(logger_name, LoggingLevel,host, port);
}

template<typename Factory = default_factory>
inline std::shared_ptr<logger> graylog_st(const std::string &logger_name, const spdlog::level::level_enum &LoggingLevel, const std::string &host, const int &port)
{
  return Factory::template create<sinks::graylog_sink_st>(logger_name, LoggingLevel, host, port);
}
}
