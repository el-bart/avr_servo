/*
 * PositionsTable.hpp
 *
 */
#ifndef INCLUDE_POSITIONSTABLE_HPP_FILE
#define INCLUDE_POSITIONSTABLE_HPP_FILE

#include "Table.hpp"

/** \brief table of positions for servos.
 */
struct PositionsTable: public Table<uint16_t, 8>
{
}; // struct PositionsTable

#endif
