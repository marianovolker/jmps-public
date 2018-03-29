/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file StreamChannelServer.h
 * @author mvolker
 * @date Monday, April 13, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _STREAMCHANNELSERVER_H_
# define _STREAMCHANNELSERVER_H_


# include <gvr/communication/IChannelServer.h>
# include <gvr/communication/IChannelServerListener.h>
# include <gvr/communication/StreamChannel.h>

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>
# include <Poco/Net/StreamSocket.h>
# include <Poco/Net/ServerSocket.h>
# include <Poco/SharedPtr.h>
# include <vector>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
        /**
         * @brief This is the class for Stream channel Server.
         * 
         * @see gvr.communication.StreamChannel
         * 
         * @since 0.0.0, 2015-04-13
         * @version 0.0.0, 2015-04-13
         * @author mvolker
         */
        class StreamChannelServer: public gvr::communication::IChannelServer
        {
          public:

            /**
             * @brief This is the status enumeration representation for the channel possible states.
             *        If you need to extend these status you could inherit from this StreamChannelServerStatus
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct StreamChannelServerStatus: public IChannelServer::IChannelServerStatus
            {
              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the channel possible errors.
             *        If you need to extend these errors you could inherit from this StreamChannelServerError
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct StreamChannelServerError: public IChannelServer::IChannelServerError
            {
              static ErrorType const CLastError;
            };

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            StreamChannelServer(const std::string& host, SizeType port);

            /**
             * @brief Destructor.
             *
             * @since 0.0.0
             */
            virtual ~StreamChannelServer (void);

            /**
             * @brief Subscribe a channel server listener to this channel server, so it can receive status events.
             *
             * @param channelserverListener is the channel server listener to be subscribed.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannelServerListener
             *
             * @since 0.0.0
             */
            virtual void subscribe (IChannelServerListener &);

            /**
             * @brief Unsubscribe a channel server listener from this channel server. It will no longer receive status events.
             *
             * @param channelserverListener is the channel server listener to be unsubscribed.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannelServerListener
             *
             * @since 0.0.0
             */
            virtual void unsubscribe (IChannelServerListener &);

            /**
             * @brief Obtain the ip configured as string.
             *
             */
            virtual std::string getIp () const {return m_host;}

            /**
             * @brief Obtain the ip configured as string.
             *
             */
            virtual int getPort () const {return m_port;}

            /**
             * @brief Do the start of this manager.
             *
             */
            virtual ErrorType start ();

            /**
            * @brief Do the stop of this manager.
            *
            */
           virtual ErrorType stop ();



          protected:


            /**
             * @brief Do the startup of the ChannelServerConnectionReceiver thread.
             *
             * @since 0.0.0
             */
            virtual bool startConnectionReceiver (void);

            /**
             * @brief Do the stop of the ChannelServerConnectionReceiver thread.
             *
             * @since 0.0.0
             */
            virtual bool stopConnectionReceiver (void);

            /**
             * @brief Wait for incoming connection to this tcp socket server.
             *
             * @param none.
             *
             * @return true if a new incoming connection is received.
             *
             * @exception Throws no exception.
             *
             *
             * @since 0.0.0
             */
            virtual void waitIncomingConnection ();

            /**
             * @brief Do the notification of connected channel to the subscribers.
             *
             * @param StreamChannel that was connected.
             *
             * @exception Throws no exception.
             *
             * @since 0.0.0
             */
            virtual void notifyStartedChannelToSubscribers (const Poco::Net::StreamSocket&);



            /**
             * @brief This subclass is a helper to create a ChannelServerConnectionReceiver thread
             * to obtain connections from the server socket.
             *
             * @see Poco::Net::StreamSocket
             * @see Poco::Runnable
             * @see Poco::Thread
             *
             * @since 0.0.0, 2014-12-16
             * @version 0.0.0, 2014-12-16
             * @author mvolker
             */
            class ChannelServerConnectionReceiver: public Poco::Runnable
            {
              public:
                ChannelServerConnectionReceiver();
                virtual ~ChannelServerConnectionReceiver();

                bool start(StreamChannelServer& channel);
                bool stop();

              private:
                bool isRunning();
                bool turnOn();
                bool turnOff();
                void run();

                // Protect the access from the outside, public interface ...
                Poco::FastMutex m_mxChannelServerConnectionReceiver;

                Poco::FastMutex m_mxChannelServer;
                StreamChannelServer* m_pStreamChannelServer;

                Poco::FastMutex m_mxRun;
                bool m_run;

                Poco::Thread m_thread;
                Poco::Event m_readyStarted;
                Poco::Event m_readyStopped;
            };


            std::string m_host;
            SizeType m_port;

            // Protect the access from the outside, public interface ...
            Poco::FastMutex m_mxStreamChannelServer;

            Poco::FastMutex m_mxConnectionReceiver;
            ChannelServerConnectionReceiver m_ChannelServerConnectionReceiver;

            Poco::FastMutex m_mxSocket;
            Poco::Net::ServerSocket m_socket;

            Poco::FastMutex m_mxListeners;
            std::set<IChannelServerListener *> m_listeners;
        };
    }
  }

#endif
