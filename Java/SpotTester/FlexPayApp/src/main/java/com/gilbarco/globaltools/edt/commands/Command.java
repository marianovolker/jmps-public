/**
 * 
 */
package com.gilbarco.globaltools.edt.commands;

import com.gilbarco.globaltools.edt.services.EdtEvent;

/**
 * @author ajadzinsky Jul 22, 2010
 * 
 */
public abstract class Command<T extends EdtEvent> {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public final void execute(T event) throws CommandExecutionException {
		try {
			CommandExecutionResult<?> result = this.beforeCommandExecution(event);
			result = this.commandExecution(event, result);
			this.afterCommandExecution(event, result);
		}
		catch (Exception e) {
			throw new CommandExecutionException(e.getMessage(), e);
		}
	}

	protected abstract CommandExecutionResult<?> beforeCommandExecution(T event) throws Exception;

	protected abstract CommandExecutionResult<?> commandExecution(T event, CommandExecutionResult<?> result) throws Exception;

	protected abstract void afterCommandExecution(T event, CommandExecutionResult<?> result) throws Exception;

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	protected class CommandExecutionResult<U> {
		private U			result;

		public CommandExecutionResult(U result) {
			this.result = result;
		}
		
		public void setResult(U result) {
			this.result = result;
		}

		public U getResult() {
			return result;
		}
	}
	
	// ******************************************************************
	// MAIN.
	// ******************************************************************
}
