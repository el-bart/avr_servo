/*
 * TimeTable.hpp
 *
 */
#ifndef INCLUDE_TIMETABLE_HPP_FILE
#define INCLUDE_TIMETABLE_HPP_FILE

#include "Table.hpp"

/** \brief table of times to activate servos at.
 */
struct TimeTable: public Table<uint16_t, 8>
{
}; // struct TimeTable

#endif
