//
// Created by paul on 29/09/23.
//

#include "duckdb/main/calculation_table.hpp"

#include "../../immersive/CalculationTable.h"

namespace duckdb {
CalculationTableWrapper::CalculationTableWrapper() {
	calc_table = duckdb::make_uniq<CalculationTable>();

}

CalculationTableWrapper::~CalculationTableWrapper() = default;

} // namespace duckdb