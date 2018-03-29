/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file StreamChannel.h
 * @author mvolker
 * @date Tuesday, Enero 06, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _STREAMCHANNEL_H_
# define _STREAMCHANNEL_H_

# include <set>
# include <vector>

# include <gvr/communication/IChannel.h>
# include <gvr/communication/IChannelListener.h>

# include <Poco/Net/StreamSocket.h>
# include <Poco/Net/SocketAddress.h>
# include <Poco/Net/NetException.h>
# include <Poco/Thread.h>
# include <Poco/Runnable.h>
# include <Poco/Event.h>
# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
        typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

        /**
         * @brief This is the class for SPOT channel.
         * 
         * @see gvr.communication.IChannel
         * 
         * @since 0.0.0, 2014-12-16
         * @version 0.0.0, 2014-12-16
         * @author mvolker
         */
        class StreamChannel: public gvr::communication::IChannel
        {
          public:

            /**
             * @brief This is the status enumeration representation for the channel possible states.
             *        If you need to extend these status you could inherit from this StreamChannelStatus
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct StreamChannelStatus: public IChannel::IChannelStatus
            {
              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the channel possible errors.
             *        If you need to extend these errors you could inherit from this StreamChannelError
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct StreamChannelError: public IChannel::IChannelError
            {
              static ErrorType const CErrorCreatingReceiver;
              static ErrorType const CErrorStoppingReceiver;

              static ErrorType const CLastError;
            };

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            StreamChannel(const std::string& host, SizeType port);

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            StreamChannel(const Poco::Net::StreamSocket&);

            /**
             * @brief Destructor.
             *
             * @since 0.0.0
             */
            virtual ~StreamChannel (void);

            /**
             * @brief Subscribe a channel listener to this channel, so it can receive status events.
             *
             * @param channelListener is the channel listener to be subscribed.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannelListener
             *
             * @since 0.0.0
             */
            virtual void subscribe (IChannelListener &);

            /**
             * @brief Unsubscribe a channel listener from this channel. It will no longer receive status events.
             *
             * @param channelListener is the channel listener to be unsubscribed.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannelListener
             *
             * @since 0.0.0
             */
            virtual void unsubscribe (IChannelListener &);

            /**
             * @brief Starts the channel communication.
             *
             * @return If and error occurs it will return the appropriate error.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannel.ErrorType
             *
             * @since 0.0.0
             */
            virtual ErrorType start (void);

            /**
             * @brief Stops the channel communication.
             *
             * @return If and error occurs it will return the appropriate error.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannel.ErrorType
             *
             * @since 0.0.0
             */
            virtual ErrorType stop (void);

            /**
             * @brief Writes the given buffer to the channel link.
             *
             * @param buffer is the buffer to be written to the channel link.
             *
             * @return If and error occurs it will return the appropriate error.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannel.ErrorType
             * @see gvr.communication.IChannel.BufferType
             *
             * @since 0.0.0
             */
            virtual ErrorType send (BufferType const &);

            /**
             * @brief Obtain the ip configured as string.
             *
             */
            virtual std::string getIp () const {return m_host;}

            /**
             * @brief Obtain the ip configured as string.
             *
             */
            virtual SizeType getPort () const {return m_port;}



          protected:

            /**
             * @brief Do the notification to the subscribers.
             *
             * @param BufferType is the data that was received.
             *
             * @exception Throws no exception.
             *
             * @since 0.0.0
             */
            virtual void notifyToSubscribers (BufferType const &);

            /**
             * @brief Do the notification to the subscribers.
             *
             * @param StatusType is the new event to report.
             *
             * @exception Throws no exception.
             *
             * @since 0.0.0
             */
            virtual void notifyToSubscribers (StatusType);

            /**
             * @brief Do the disconnect to the socket.
             *
             * @since 0.0.0
             */
            virtual void disconnect (void);

            /**
             * @brief Do the connect to the socket.
             *
             * @since 0.0.0
             */
            virtual void connect (void);

            /**
             * @brief Do the startup of the ChannelReceiver thread.
             *
             * @since 0.0.0
             */
            virtual bool startReceiver (void);

            /**
             * @brief Do the stop of the ChannelReceiver thread.
             *
             * @since 0.0.0
             */
            virtual bool stopReceiver (void);

            /**
             * @brief Init status.
             *
             * @since 0.0.0
             */
            virtual void init (void);

            /**
             * @brief Obtain data from the buffer of the channel link.
             *
             * @param none.
             *
             * @return true if data is received.
             *
             * @exception Throws no exception.
             *
             *
             * @since 0.0.0
             */
            virtual void receive ();


          protected:

            /**
             * @brief This subclass is a helper to create a ChannelReceiver thread
             * to obtain data from the socket.
             *
             * @see Poco::Net::StreamSocket
             * @see Poco::Runnable
             * @see Poco::Thread
             *
             * @since 0.0.0, 2014-12-16
             * @version 0.0.0, 2014-12-16
             * @author mvolker
             */
            class ChannelReceiver: public Poco::Runnable
            {
              public:
                ChannelReceiver();
                virtual ~ChannelReceiver();

                bool start(StreamChannel& channel);
                bool stop();

              private:
                bool isRunning();
                bool turnOn();
                bool turnOff();
                void run();

                // Protect the access from the outside, public interface ...
                Poco::FastMutex m_mxChannelReceiver;

                Poco::FastMutex m_mxChannel;
                StreamChannel* m_pStreamChannel;

                Poco::FastMutex m_mxRun;
                bool m_run;

                Poco::Thread m_thread;
                Poco::Event m_readyStarted;
                Poco::Event m_readyStopped;
            };


          protected:

            std::string m_host;
            SizeType m_port;

            // Protect the access from the outside, public interface ...
            Poco::FastMutex m_mxStreamChannel;

            Poco::FastMutex m_mxReceiver;
            ChannelReceiver m_channelReceiver;

            Poco::FastMutex m_mxSocket;
            Poco::Net::StreamSocket m_socket;

            Poco::FastMutex m_mxListeners;
            std::set<IChannelListener *> m_listeners;

            bool m_socketConnected;
        };
    }
  }

#endif
