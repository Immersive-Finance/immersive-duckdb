//
// Created by paul on 29/09/23.
//

#ifndef DUCKDB_CALCULATION_TABLE_HPP
#define DUCKDB_CALCULATION_TABLE_HPP

#include "../../../../immersive/CalculationTable.h"
namespace duckdb {
class AttachedDatabase;
class ClientContext;
class DatabaseInstance;

class CalculationTableWrapper {

public:
	DUCKDB_API explicit CalculationTableWrapper();
	DUCKDB_API ~CalculationTableWrapper();

	//so examining the code, it looks like the CalculationTableWrapper is a wrapper for the CalculationTable class
	//so lets wrap the create_table function
	DUCKDB_API void create_table() {
		calc_table->create_table();
	}

	DUCKDB_API void insert_row(int scenario_id, int trade_id, double notional, int cp, double K, double T,
	                double sqrt_T, double spot, double fwd, double vol, std::string greek) {
		calc_table->insert_row(scenario_id, trade_id, notional, cp, K, T, sqrt_T, spot, fwd, vol, greek);
	}

	DUCKDB_API void calculate() {
		calc_table->calculate();
	}

	DUCKDB_API void append_rows(const std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>> &rows) {
		calc_table->append_rows(rows);
	}

	DUCKDB_API duckdb::shared_ptr<Connection> get_conn() {
		return calc_table->getConn();
	}

	DUCKDB_API std::string get_dbname() {
		return calc_table->getDbname();
	}

	DUCKDB_API std::string get_table_name() {
		return calc_table->getTableName();
	}


private:
	unique_ptr<CalculationTable> calc_table;

};


} // namespace duckdb

#endif // DUCKDB_CALCULATION_TABLE_HPP
