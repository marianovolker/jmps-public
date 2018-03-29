/**
 * 
 */
package com.gilbarco.globaltools.edt.commands;

import java.util.HashMap;
import java.util.Map;

import com.gilbarco.globaltools.edt.services.EdtEvent;

/**
 * @author ajadzinsky Jul 22, 2010
 *
 */
public final class CommandRegistry {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final CommandRegistry instance = new CommandRegistry();
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	public static CommandRegistry getInstance(){
		return instance;
	}
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private Map<Enum<?>, Command<? extends EdtEvent>> registry;
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private CommandRegistry(){
		this.registry = new HashMap<Enum<?>, Command<? extends EdtEvent>>();
//		@author slipchak
//		example code on how to use the registry see @code mgp-app project
//		this.registry.put(ProtocolEventType.NEW, new ProtocolNewCommand());
//		this.registry.put(ProtocolEventType.OPEN, new ProtocolOpenCommand());
//		this.registry.put(ProtocolEventType.CLOSE, new ProtocolCloseCommand());
//		this.registry.put(ProtocolEventType.CLOSE_ALL, new ProtocolCloseCommand());
//		this.registry.put(ProtocolEventType.CLOSE_MSSGS, new ProtocolCloseCommand());
//		this.registry.put(ProtocolEventType.SAVE, new ProtocolSaveCommand());
//		this.registry.put(TreeElementEventType.SELECT, new SelectElementCommand());
//		this.registry.put(TreeElementEventType.REMOVE, new RemoveElementCommand());
//		this.registry.put(TreeElementEventType.ADD_MESSAGE, new AddMessageCommand());
//		this.registry.put(TreeElementEventType.ADD_HOLDER_CONTAINER, new AddHolderContainerCommand());
//		this.registry.put(TreeElementEventType.ADD_CONTAINER, new AddContainerCommand());
//		this.registry.put(TreeElementEventType.ADD_LINK, new AddLinkCommand());
//		this.registry.put(TreeElementEventType.ADD_FIELD, new AddFieldCommand());
//		this.registry.put(TreeElementEventType.MOVE_UP, new MoveElementCommand());
//		this.registry.put(TreeElementEventType.MOVE_DOWN, new MoveElementCommand());
//		this.registry.put(MessageEventType.OPEN, new MessageOpenCommand());
	}
	// ******************************************************************
	// PUBLIC METHODS       (general, getter, setter, interface imp)
	// ******************************************************************
	@SuppressWarnings("unchecked")
	public <T extends EdtEvent> Command<T> getCommand(Enum<?> type) {
		return (Command<T>) this.registry.get(type);
	}
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
