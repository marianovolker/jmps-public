//## begin module%357E88BF0019.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%357E88BF0019.cm

//## begin module%357E88BF0019.cp preserve=no
//## end module%357E88BF0019.cp

//## Module: GAbsCommand%357E88BF0019; Package specification
//## Subsystem: Common::Util%359176090340
//## Source file: src\Common\Util\GAbsCommand.h

#ifndef GAbsCommand_H
#define GAbsCommand_H 1

//## begin module%357E88BF0019.additionalIncludes preserve=no
//## end module%357E88BF0019.additionalIncludes

//## begin module%357E88BF0019.includes preserve=yes
//## end module%357E88BF0019.includes

//## begin module%357E88BF0019.declarations preserve=no
//## end module%357E88BF0019.declarations

//## begin module%357E88BF0019.additionalDeclarations preserve=yes
//## end module%357E88BF0019.additionalDeclarations


//## begin GAbsCommand%357E88BF0069.preface preserve=yes
//## end GAbsCommand%357E88BF0069.preface

//## Class: GAbsCommand%357E88BF0069; Abstract
//	=============================================================================
//
//	File: GAbsCommand.h
//
//	Description: Contains the GAbsCommand class definition. This is an abstract
//	class to be used as a base class for commands.
//
//	Created: 02/26/98
//
//	Author: Jon Towne
//
//	=============================================================================
//	-----------------------------------------------------------------------------
//	This class implements the command pattern. It encapsulates a method call
//	on an associated object. This class is an abstract class and as such there
//	is no corresponding implementation file. It is only meant to provide a base
//	class for commands.
//## Category: CommonLP::UtilLP%35901B900282; Global
//## Subsystem: Common::Util%359176090340
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class GAbsCommand 
{
  //## begin GAbsCommand%357E88BF0069.initialDeclarations preserve=yes
  //## end GAbsCommand%357E88BF0069.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: GAbsCommand%-2129467092; C++
      //	Constructor.
      GAbsCommand ()
        //## begin GAbsCommand::GAbsCommand%-2129467092.hasinit preserve=no
        //## end GAbsCommand::GAbsCommand%-2129467092.hasinit
        //## begin GAbsCommand::GAbsCommand%-2129467092.initialization preserve=yes
        //## end GAbsCommand::GAbsCommand%-2129467092.initialization
      {
        //## begin GAbsCommand::GAbsCommand%-2129467092.body preserve=yes
        //## end GAbsCommand::GAbsCommand%-2129467092.body
      }

    //## Destructor (specified)
      //## Operation: ~GAbsCommand%-339850003; C++
      //	Destructor.
      virtual ~GAbsCommand ()
      {
        //## begin GAbsCommand::~GAbsCommand%-339850003.body preserve=yes
        //## end GAbsCommand::~GAbsCommand%-339850003.body
      }


    //## Other Operations (specified)
      //## Operation: Execute%-1250409779; C++
      //	Executes the command.
      virtual void Execute ()
      {
        //## begin GAbsCommand::Execute%-1250409779.body preserve=yes
        //## end GAbsCommand::Execute%-1250409779.body
      }

    // Additional Public Declarations
      //## begin GAbsCommand%357E88BF0069.public preserve=yes
      //## end GAbsCommand%357E88BF0069.public

  protected:
    // Additional Protected Declarations
      //## begin GAbsCommand%357E88BF0069.protected preserve=yes
      //## end GAbsCommand%357E88BF0069.protected

  private:
    // Additional Private Declarations
      //## begin GAbsCommand%357E88BF0069.private preserve=yes
      //## end GAbsCommand%357E88BF0069.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin GAbsCommand%357E88BF0069.implementation preserve=yes
      //## end GAbsCommand%357E88BF0069.implementation

};

//## begin GAbsCommand%357E88BF0069.postscript preserve=yes
//## end GAbsCommand%357E88BF0069.postscript

//## begin module%357E88BF0019.epilog preserve=yes
//## end module%357E88BF0019.epilog


#endif
