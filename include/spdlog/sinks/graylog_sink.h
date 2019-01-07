#pragma once

#ifndef SPDLOG_H
#error "spdlog.h must be included before this file."
#endif

#include "spdlog/sinks/base_sink.h"
#include <graylog_logger/Logger.hpp>
#include <map>
#include <mutex>

namespace spdlog {
namespace sinks {

template <typename Mutex>
class graylog_sink : public spdlog::sinks::base_sink<Mutex> {
private:
  std::map<level::level_enum, Log::Severity> LogLevels{
      {SPDLOG_LEVEL_TRACE, Log::Severity::Debug},
      {SPDLOG_LEVEL_DEBUG, Log::Severity::Info},
      {SPDLOG_LEVEL_INFO, Log::Severity::Notice},
      {SPDLOG_LEVEL_WARN, Log::Severity::Warning},
      {SPDLOG_LEVEL_ERROR, Log::Severity::Error},
      {SPDLOG_LEVEL_CRITICAL, Log::Severity::Critical},
      {SPDLOG_LEVEL_OFF, Log::Severity::Alert}
  };

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    auto Level = LogLevels[msg.level];
    LogLevels Log::Msg(Level, msg.payload);
  }
  void flush_() override {}
};
}
}
