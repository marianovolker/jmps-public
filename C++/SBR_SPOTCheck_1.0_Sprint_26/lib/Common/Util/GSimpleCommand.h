//## Module: GSimpleCommand%357E88C100D9; Package specification
//## Subsystem: Common::Util%359176090340
//## Source file: src\Common\Util\GSimpleCommand.h

#ifndef GSimpleCommand_H
#define GSimpleCommand_H 1

//## begin module%357E88C100D9.includes preserve=yes
//## end module%357E88C100D9.includes

// GAbsCommand
#include "Common/Util/GAbsCommand.h"
//## begin module%357E88C100D9.additionalDeclarations preserve=yes
//## end module%357E88C100D9.additionalDeclarations


//## begin GSimpleCommand%357E88C100E3.preface preserve=yes
//## end GSimpleCommand%357E88C100E3.preface

//## Class: GSimpleCommand%357E88C100E3; Parameterized Class
//	-----------------------------------------------------------------------------
//	This class implements the command pattern as a template. In order to
//	encapsulate a method call on an object, 3 things are needed. You need
//	to know the class of the object which the method is to be called on.
//	You also need to know the instance of the class and what method to
//	call. The way this is handled in this class is that the class is
//	specified as a template parameter and the class instance and method are
//	specified in the constructor.
//	An alternate usage is to construct the command with the default constructor
//	and use SetCommand to complete the association later.
//	Using the SetCommand method allows for reuse of a simple command to call
//	a different object and/or method .
//## Category: CommonLP::UtilLP%35901B900282; Global
//## Subsystem: Common::Util%359176090340
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class Receiver>
class GSimpleCommand : public GAbsCommand  //## Inherits: <unnamed>%357E88C1010D
{
  //## begin GSimpleCommand%357E88C100E3.initialDeclarations preserve=yes
  public:
	typedef void (Receiver::* Action) (void);

  //## end GSimpleCommand%357E88C100E3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: GSimpleCommand%-564158229; C++
      //	Default constructor. If this constructor is used, the SetCommand
      //	method will need to be called to associate the command with a method
      //	on a particular instance.
      GSimpleCommand ()
        //## begin GSimpleCommand::GSimpleCommand%-564158229.hasinit preserve=yes
  :
	m_receiver (0), 
	m_action   (0)
        //## end GSimpleCommand::GSimpleCommand%-564158229.hasinit
        //## begin GSimpleCommand::GSimpleCommand%-564158229.initialization preserve=yes
        //## end GSimpleCommand::GSimpleCommand%-564158229.initialization
      {
        //## begin GSimpleCommand::GSimpleCommand%-564158229.body preserve=yes
        //## end GSimpleCommand::GSimpleCommand%-564158229.body
      }

      //## Operation: GSimpleCommand%277948286; C++
      //	Constructor which allows the specification of instance and
      //	method to execute.
      //	Some compilers have a bug that will give you an unresolved
      //	external error if you try to use this. If this happens,
      //	use the default constructor, followed by a call to SetCommand.
      GSimpleCommand (Receiver* receiver, Action action)
        //## begin GSimpleCommand::GSimpleCommand%277948286.hasinit preserve=yes
  :
	m_receiver (receiver),
	m_action   (action)
        //## end GSimpleCommand::GSimpleCommand%277948286.hasinit
        //## begin GSimpleCommand::GSimpleCommand%277948286.initialization preserve=yes
        //## end GSimpleCommand::GSimpleCommand%277948286.initialization
      {
        //## begin GSimpleCommand::GSimpleCommand%277948286.body preserve=yes


		// Constructor which allows the specification of instance and 
		// method to execute.
		//
        //## end GSimpleCommand::GSimpleCommand%277948286.body
      }


    //## Other Operations (specified)
      //## Operation: Execute%-1187334174; C++
      //	Executes the associated method on the associated instance.
      virtual void Execute ()
      {
        //## begin GSimpleCommand::Execute%-1187334174.body preserve=yes

		// Executes the associated method on the associated instance.
		//

			// Make sure that the instance has been specified, and
			//
			if (m_receiver) 
			{
				// the method has been specified.
				//
				if (m_action)
				{
					// Call the method 
					//
					(m_receiver->*m_action)();
				}
			}

				//## end GSimpleCommand::Execute%-1187334174.body
			  }

      //## Operation: SetCommand%-2018946498; C++
      //	Sets the object and method. This method can be used more
      //	than once to swap out the associated instance and/or
      //	method.
      void SetCommand (Receiver* receiver, Action action)
      {
        //## begin GSimpleCommand::SetCommand%-2018946498.body preserve=yes

		// Sets the object and method. This method can be used more
		// than once to swap out the associated instance and/or 
		// method.
		//

			m_receiver = receiver; 
			m_action   = action;

        //## end GSimpleCommand::SetCommand%-2018946498.body
      }

  private:
    //## Constructors (generated)
      GSimpleCommand(const GSimpleCommand< Receiver > &right);

    //## Assignment Operation (generated)
      const GSimpleCommand< Receiver > & operator=(const GSimpleCommand< Receiver > &right);

    // Data Members for Class Attributes

      //## Attribute: m_receiver%357E88C10103
      //## begin GSimpleCommand::m_receiver%357E88C10103.attr preserve=no  private: Receiver* {VA} 
      Receiver* m_receiver;
      //## end GSimpleCommand::m_receiver%357E88C10103.attr

      //## Attribute: m_action%357E88C10107
      //	Associated instance.
      //## begin GSimpleCommand::m_action%357E88C10107.attr preserve=no  private: Action {VA} 
      Action m_action;
      //## end GSimpleCommand::m_action%357E88C10107.attr

};


#endif
