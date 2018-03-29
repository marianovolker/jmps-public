//## begin module%366662200392.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%366662200392.cm

//## begin module%366662200392.cp preserve=no
//## end module%366662200392.cp

//## Module: GAbsMessCommand%366662200392; Package specification
//## Subsystem: Common::Util%359176090340
//## Source file: z:\ngd_pump\source\Common\Util\GAbsMessCommand.h

#ifndef GAbsMessCommand_h
#define GAbsMessCommand_h 1

//## begin module%366662200392.additionalIncludes preserve=no
//## end module%366662200392.additionalIncludes

//## begin module%366662200392.includes preserve=yes
//## end module%366662200392.includes


class GMessage;

//## begin module%366662200392.declarations preserve=no
//## end module%366662200392.declarations

//## begin module%366662200392.additionalDeclarations preserve=yes
//## end module%366662200392.additionalDeclarations


//## begin GAbsMessCommand%36654ECB005D.preface preserve=yes
//## end GAbsMessCommand%36654ECB005D.preface

//## Class: GAbsMessCommand%36654ECB005D; Abstract
//	Abstract interface for a command that  take a GMessage * in the Execute
//	method as parameter.
//## Category: CommonLP::UtilLP%35901B900282; Global
//## Subsystem: Common::Util%359176090340
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%366663670193;GMessage { -> F}

class GAbsMessCommand 
{
  //## begin GAbsMessCommand%36654ECB005D.initialDeclarations preserve=yes
  //## end GAbsMessCommand%36654ECB005D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: GAbsMessCommand%912679125; C++
      //	Constructor.
      GAbsMessCommand ();

    //## Destructor (specified)
      //## Operation: ~GAbsMessCommand%912679126; C++
      //	Destructor.
      virtual ~GAbsMessCommand ();


    //## Other Operations (specified)
      //## Operation: Execute%912679127; C++
      virtual void Execute (GMessage* pGMessage) = 0;

    // Additional Public Declarations
      //## begin GAbsMessCommand%36654ECB005D.public preserve=yes
      //## end GAbsMessCommand%36654ECB005D.public

  protected:
    // Additional Protected Declarations
      //## begin GAbsMessCommand%36654ECB005D.protected preserve=yes
      //## end GAbsMessCommand%36654ECB005D.protected

  private:
    // Additional Private Declarations
      //## begin GAbsMessCommand%36654ECB005D.private preserve=yes
      //## end GAbsMessCommand%36654ECB005D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin GAbsMessCommand%36654ECB005D.implementation preserve=yes
      //## end GAbsMessCommand%36654ECB005D.implementation

};

//## begin GAbsMessCommand%36654ECB005D.postscript preserve=yes
//## end GAbsMessCommand%36654ECB005D.postscript

// Class GAbsMessCommand 

//## Operation: GAbsMessCommand%912679125; C++
//## Semantics:
//	========================================================================
//	Name: AbsMessCommand
//
//	Purpose: Constructor.
//
//	Returns: AbsMessCommand
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
inline GAbsMessCommand::GAbsMessCommand ()
  //## begin GAbsMessCommand::GAbsMessCommand%912679125.hasinit preserve=no
  //## end GAbsMessCommand::GAbsMessCommand%912679125.hasinit
  //## begin GAbsMessCommand::GAbsMessCommand%912679125.initialization preserve=yes
  //## end GAbsMessCommand::GAbsMessCommand%912679125.initialization
{
  //## begin GAbsMessCommand::GAbsMessCommand%912679125.body preserve=yes
  //## end GAbsMessCommand::GAbsMessCommand%912679125.body
}


//## Operation: ~GAbsMessCommand%912679126; C++
//## Semantics:
//	========================================================================
//	Name: ~AbsMessCommand
//
//	Purpose: Destructor.
//
//	Returns: AbsMessCommand
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
inline GAbsMessCommand::~GAbsMessCommand ()
{
  //## begin GAbsMessCommand::~GAbsMessCommand%912679126.body preserve=yes
  //## end GAbsMessCommand::~GAbsMessCommand%912679126.body
}


//## begin module%366662200392.epilog preserve=yes
//## end module%366662200392.epilog


#endif
