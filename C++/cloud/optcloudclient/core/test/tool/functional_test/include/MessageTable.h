/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MessageTable.h
 * @author gparis
 * @date Tuesday, May 04, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef MESSAGETABLE_H_
#define MESSAGETABLE_H_

#include <string>
#include <map>


/// @brief Gilbarco Domain
namespace gvr
{
/// @brief Cloud Domain
namespace cloud
{


  class MessageTable
  {
  public:

    struct TableEntry
    {
        std::string  sCommandClass;
        std::string  sCommandOperation;
        int          iCommandFlag;
        std::string  sCommandReplyFilename;

        TableEntry() : iCommandFlag(0), sCommandClass(), sCommandOperation(), sCommandReplyFilename() {}
    };

    typedef std::map<std::string, TableEntry>                 TableMap; ///< @brief Map of entries.
    typedef std::map<std::string, TableEntry>::iterator       TableIterator; ///< @brief Map iterator
    typedef std::map<std::string, TableEntry>::const_iterator TableConstIterator; ///< @brief const Map iterator.
    typedef std::pair<std::string,TableEntry>                 EntryPair; ///< @brief Key-TableEntry pair.
    typedef std::pair<TableIterator,bool>                     InsertRetPair; ///< @brief Pair returned by insert()

    /**
     * @brief Default Constructor
     */
    MessageTable(void);

    /**
     * @brief Constructor
     */
    MessageTable(const std::string & sFullFileName );

    /**
     * @brief Destructor
     */
    ~MessageTable (void);

    /**
     * @brief Called externally this loads the complete message table into a global memory object.
     */
    void LoadTable (void);

    /**
     * @brief Checks if the table could be properly loaded with at least one record.
     */
    bool hasError (void);

    /**
     * @brief Gets the table entry corresponding to a given key;
     */
    const TableEntry* getTableEntry(const std::string & sKey);


  private:

    static const int     CMinCharsInLine = 9;///< @brief Minimum quantity of chars per line allowed.
    static const char *  CDefaultPathName;   ///< @brief Default full pathname.

    bool            m_blError;      ///< @brief Table is in error state;
    std::string     m_sFullFileName;///< @brief Full filename to the text table.
    TableMap        m_Map;          ///< @brief Map of commands and optional responses.
    Poco::FastMutex m_Mutex;        ///< @brief Mutex for sharing the map between threads.
  };


} // Cloud Domain
} // Gilbarco Domain


#endif /* MESSAGETABLE_H_ */
