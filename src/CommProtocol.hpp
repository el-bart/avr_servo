#ifndef INCLUDE_COMMPROTOCOL_HPP_FILE
#define INCLUDE_COMMPROTOCOL_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "CommQueues.hpp"
#include "Noncopyable.hpp"
#include "ChronoTable.hpp"

class CommProtocol: private Noncopyable
{
public:
  CommProtocol(QueueSend &qSend, QueueRecv &qRecv, PersistentSettings &settings);

  /** \brief updates basing on protocol data.
   *  \param posTab positions to update.
   */
  void process(ChronoTable::Positions &posTab);

private:
  void skipUntilNewCommand(void);
  void skipEndMarkers(void);
  bool execute(uint8_t srvNo, char mode, uint8_t pos, ChronoTable::Positions &posTab);
  void replyOk(char srvName);
  void replyError(char srvName);

  QueueSend          &qSend_;
  QueueRecv          &qRecv_;
  PersistentSettings &settings_;
}; // class CommProtocol

#endif
