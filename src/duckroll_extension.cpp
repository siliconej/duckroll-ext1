#define DUCKDB_EXTENSION_MAIN

#include "duckroll_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void DuckrollScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Duckroll "+name.GetString()+" 🐥");;
        });
}

inline void DuckrollOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Duckroll " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto duckroll_scalar_function = ScalarFunction("duckroll", {LogicalType::VARCHAR}, LogicalType::VARCHAR, DuckrollScalarFun);
    ExtensionUtil::RegisterFunction(instance, duckroll_scalar_function);

    // Register another scalar function
    auto duckroll_openssl_version_scalar_function = ScalarFunction("duckroll_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, DuckrollOpenSSLVersionScalarFun);
    ExtensionUtil::RegisterFunction(instance, duckroll_openssl_version_scalar_function);
}

void DuckrollExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string DuckrollExtension::Name() {
	return "duckroll";
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void duckroll_init(duckdb::DatabaseInstance &db) {
	LoadInternal(db);
}

DUCKDB_EXTENSION_API const char *duckroll_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
