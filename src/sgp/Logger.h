#ifndef SGP_LOGGER_H_
#define SGP_LOGGER_H_

#include "Platform.h"
#include "RustInterface.h"
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>
#include <string_theory/format>
#include <string_theory/string>

/** Get filename relative to src directory */
constexpr size_t GetSourcePathSize(const char* filename)
{
	std::string_view s(filename);
	size_t i = s.rfind(SOURCE_ROOT);

	return (i != std::string::npos)
		? i + std::string_view(SOURCE_ROOT).length()
		: 0;
}

template <size_t p> constexpr const char* ToRelativePath(const char* filename)
{	// this forces compile-time evaluation of p; we don't have consteval yet
	return &filename[p];
}

// Determine the src path at compile time, for performant log calls to Rust that use non-absolute file name
#define SOURCE_PATH_SIZE (GetSourcePathSize(__FILE__))
#define __FILENAME__ (ToRelativePath<SOURCE_PATH_SIZE>(__FILE__))

/** Per-file debug log filter.
 *
 * Controlled by the JA2_LOG_FILTER environment variable: a comma-separated list
 * of path fragments (e.g. "TacticalAI/DecideAction.cc,TacticalAI/AIUtils.cc").
 * When set, Debug/Trace messages are only emitted from files whose relative path
 * contains one of the fragments; Info/Warn/Error messages always pass so that
 * important diagnostics are never hidden. When unset/empty, every file passes
 * (legacy behavior). The env var is parsed once on first use. */
inline bool LogFilterAllowsFile(LogLevel level, const char* file)
{
	// Only Debug/Trace are noisy enough to filter; let everything else through.
	if (level < LogLevel::Debug) {
		return true;
	}

	static const std::vector<std::string> filter = []
	{
		std::vector<std::string> result;
		const char* env = std::getenv("JA2_LOG_FILTER");
		if (env != nullptr && env[0] != '\0') {
			std::string_view s(env);
			size_t start = 0;
			while (start <= s.size()) {
				size_t comma = s.find(',', start);
				if (comma == std::string_view::npos) comma = s.size();
				std::string_view tok = s.substr(start, comma - start);
				while (!tok.empty() && tok.front() == ' ') tok.remove_prefix(1);
				while (!tok.empty() && tok.back()  == ' ') tok.remove_suffix(1);
				if (!tok.empty()) {
					// Normalize separators so the env var can use either
					// '/' or '\' regardless of the platform's __FILE__ style.
					std::string frag(tok);
					for (char& c : frag) if (c == '\\') c = '/';
					result.emplace_back(std::move(frag));
				}
				start = comma + 1;
			}
		}
		return result;
	}();

	if (filter.empty()) {
		return true; // no filter configured -> show everything
	}

	// Normalize the file path the same way before matching, because
	// __FILE__ uses backslashes on MSVC builds (see SOURCE_ROOT).
	std::string f(file);
	for (char& c : f) if (c == '\\') c = '/';
	for (const std::string& fragment : filter) {
		if (f.find(fragment) != std::string::npos) {
			return true;
		}
	}
	return false;
}

template<typename... Args>
constexpr void LogMessageST([[maybe_unused]] bool isAssert, LogLevel level, const char* file, Args && ... args)
{
	if (level <= Logger_getLevel() && LogFilterAllowsFile(level, file)) {
		Logger_log(level, ST::format(std::forward<Args>(args)...).c_str(), file);
	}

	#ifdef ENABLE_ASSERTS
	if (isAssert)
	{
		abort();
	}
	#endif
}

/** Print debug message macro. */
#define SLOGD(...) LogMessageST(false, LogLevel::Debug, __FILENAME__, ##__VA_ARGS__)

/** Print info message macro. */
#define SLOGI(...) LogMessageST(false, LogLevel::Info,  __FILENAME__, ##__VA_ARGS__)

/** Print warning message macro. */
#define SLOGW(...) LogMessageST(false, LogLevel::Warn, __FILENAME__, ##__VA_ARGS__)

/** Print error message macro. */
#define SLOGE(...) LogMessageST(false, LogLevel::Error, __FILENAME__, ##__VA_ARGS__)

/** Print error message macro and assert if ENABLE_ASSERTS is defined. */
#define SLOGA(...) LogMessageST(true, LogLevel::Error, __FILENAME__, ##__VA_ARGS__)

#endif//SGP_LOGGER_H_
