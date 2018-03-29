
package com.gilbarco.globaltools.flexpay.rkl.state;

import java.util.Map;
import java.util.Queue;

import com.gilbarco.globaltools.flexpay.rkl.rklexception.RklException;
import com.gilbarco.globaltools.rklnativeprotocol.api.RklNativeMessageType;

public enum RKLNativeStates
{
	IDLE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map)
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 0;
		}

		@Override
		public String getName()
		{
			return "IDLE";
		}
	},
	GET_SPOT_IDENTIFICATION
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map)
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 0;
		}

		@Override
		public String getName()
		{
			return "Get Spot Identification";
		}
	},
	SEND_SPOT_IDENTIFICATION
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			queue.clear( );
			if (!map.isEmpty( ))
			{
				queue.add( (byte[]) map.get( "SerialNumber" ) );
			}
			return;
		}

		@Override
		public int getStep()
		{
			return 1;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.SEND_IDENTIFICATION.name( );
		}
	},
	GET_SPOT_CERTIFICATE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 2;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.CERTIFICATE_IDENTIFICATION.name( );
		}
	},
	CONTACT_GILBARCO
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 3;
		}

		@Override
		public String getName()
		{
			return "CONTACT_GILBARCO";
		}
	},

	SEND_SPOT_CERTIFICATE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			queue.clear( );
			if (!map.isEmpty( ))
			{
				queue.add( (byte[]) map.get( "SpotCertificate" ) );
			}
			return;
		}

		@Override
		public int getStep()
		{
			return 4;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.CERTIFICATE_IDENTIFICATION.name( );
		}
	},
	SEND_SPOT_CERTIFICATE_VERIFICATION
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			queue.clear( );
			if (!map.isEmpty( ))
			{
				queue.add( (byte[]) map.get( "CertificateVerification" ) );
			}
			return;
		}

		@Override
		public int getStep()
		{
			return 5;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.CERTIFICATE_IDENTIFICATION.name( );
		}
	},
	GET_KMS_HOST_CERTIFICATE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 6;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.REMOTE_CERTIFICATE_REQUEST.name( );
		}
	},
	SEND_HOST_CERTIFICATE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 7;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.REMOTE_CERTIFICATE_REQUEST.name( );
		}
	},
	GET_SPOT_NONCE_DATA
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 8;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.REMOTE_CERTIFICATE_REQUEST.name( );
		}
	},
	REQUEST_KEY_MANAGEMENT
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 9;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.KEY_MANAGEMENT_REQUEST.name( );
		}
	},
	SEND_KEY_MANAGEMENT_DATA
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 9;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.KEY_MANAGEMENT_REQUEST.name( );
		}
	},

	GET_KEYS
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 10;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.KEY_REQUEST.name( );
		}
	},
	SEND_KEY_RESPONSE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 11;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.KEY_REQUEST.name( );
		}
	},
	SEND_CLOSE_KEY_SESSION
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 12;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.CLOSE_KEY_VERIFICATION_REQUEST.name( );
		}
	},
	SEND_CLOSE_KEY_SESSION_ANSWER
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 13;
		}

		@Override
		public String getName()
		{
			return RklNativeMessageType.CLOSE_KEY_VERIFICATION_REQUEST.name( );
		}
	},
	NOTIFY_ERROR
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 14;
		}

		@Override
		public String getName()
		{
			return "NOTIFY_ERROR";
		}
	},
	FINAL_STATE
	{
		@Override
		public void processEventProperties(Queue<byte[]> queue, Map<String, Object> map)
		{
			return;
		}

		@Override
		public int getStep()
		{
			return 15;
		}

		@Override
		public String getName()
		{
			return "FINAL_STATE";
		}
	};

	public abstract void processEventProperties(Queue<byte[]> queue, Map<String, Object> map) throws RklException;

	public static RKLNativeStates getStateByName(String name)
	{
		for (RKLNativeStates state : RKLNativeStates.values( ))
			if (state.name( ).equals( name )) return state;

		return null;
	}

	public abstract int getStep();

	public abstract String getName();

	public static int	TOTAL_STATES	= 12;
}
