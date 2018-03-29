/*!
 * \file GCharMapping.h
 *
 * \author guillermo tomasini
 * \date	09-21-2009
 *
 * utf8 mapping (ascii to utf8) from text file
 */

#ifndef GCharMapping_H_
#define GCharMapping_H_

#include <QString>
#include <QChar>
#include <QVector>

#include "GCharMappingException.h"
#include "utils/configreader/src/GConfigReader.h"

/*!
 * \class GCharMapping
 *
 * \brief mapping from ascii to utf8 from text file
 *
 * mapping from ascii to utf8 from text file
 * each line in the text file has the form:
 *
 * char_000=&#x0;
 * char_001=&#x1;
 * char_002=&#x2;
 * char_003=&#xe4;
 * char_004=&#33;
 * ....
 * char_255=&#255;
 * 
 * where &#x is an hexadecimal value and &# is a decimal value
 * 
 * \author guillermo tomasini
 * \date	09-21-2009
 */


class GCharMapping
{
	
public:

	/*!
	* \brief constructor
	*
	*
	* \param[in] mapFilename text file name with the map
	*/
	GCharMapping( const QString &mapFilename="") throw (GCharMappingException);

	/*!
	* \brief copy constructor
	*
	*
	* \param[in] GCharMapping object reference
	*/
	GCharMapping ( const GCharMapping & );

	/*!
	* \brief equal operator
	*
	*
	* \param[in] GCharMapping object reference
	*/
	GCharMapping & operator =( const GCharMapping &);

	/*!
	* \brief destructor
	*
	*/
	virtual ~GCharMapping();

	/*!
	* \brief Initialization
	*
	* reads from mapFilename the ascii->utf8 map.
	*
	* \param[in] mapFilename 
	* 
	*/
	void Init( const QString &mapFilename="") throw (GCharMappingException);

	/*!
	* \brief Reset
	*
	* sets the vector to 0.
	*
	*
	*/
	void Reset();

	/*!
	* \brief getter
	*
	* return map Vector
	*
	* \return QVector<QChar>
	*/
	QVector<QChar> & GetCharVec();

	/*!
	* \brief getter
	*
	* return the ith Qchar
	*
	* \param[in] i index en map
	*
	* \return QChar
	*/
	QChar & GetQChar( int i );
	
private:

	GConfigReader* m_pConfig;
	QVector<QChar> m_CharVec;

	enum { eLanguageMappingTableSize = 256 };
	enum { eLanguageMappingNullCharacter = 0x0000 };

};

#endif
