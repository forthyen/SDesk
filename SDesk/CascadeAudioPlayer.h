//
// CascadeAudioPlayer.h - header file for class CascadeAudioPlayer
//
// Copyright (c) 2004, Roku, LLC.  All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
/// @file CascadeAudioPlayer.h header file of class CascadeAudioPlayer

#ifndef _ROKU_INCLUDE_CASCADE_AV_CASCADEAUDIOPLAYER_H
#define _ROKU_INCLUDE_CASCADE_AV_CASCADEAUDIOPLAYER_H

#include <cascade/av/CascadeAudioCodec.h>
#include <cascade/av/CascadeAudioPlaybackDevice.h>
#include <cascade/util/CascadeStream.h>

#ifndef DOXY_SKIP
    namespace CascadePrivate { class CascadeAudioPlayerInitializer; };
#endif

///////////////////////////////////////////////////////////////////////////////
/// @class CascadeAudioPlayer CascadeAudioPlayer.h <cascade/av/CascadeAudioPlayer.h>
/// @brief the Cascade way to playback audio
///
/// <a href="#themeat"><b>Skip the description</b></a>
///
/// CascadeAudioPlayer is a class that allows easy playback of audio streams
/// of all sources, types, and durations.
///
/// Use: CascadeStream stream;
///      CascadeAudioPlayer player;
///      player.PlayStream(stream);
///
/// <hr><a name=themeat></a>
/// <h2>class CascadeAudioPlayer</h2>
class CascadeAudioPlayer : public CascadeObject
{
public: class Client;   // forward declaration
public:
	CascadeAudioPlayer();
        ///<the default constructor - lightweight
        ///
        /// This default constructor is lightweight.
	virtual ~CascadeAudioPlayer();
        ///<destructor
        ///
        /// The destructor.
public:
    virtual bool Open(Client * pClient = NULL);
        ///<opens the audio player
        ///
        /// %Open() opens the audio player.  The player must be opened succesfully before
        /// it can be used.  If %Open() fails, that means that another client has the
        /// audio player opened.
        ///
        /// @param pClient the client to receive notifications
        /// @return whether or not the open succeeded
    virtual void Close();
        ///<closes the audio player
        ///
        /// %Close() closes the audio player.  Be sure to close (or delete) the audio player
        /// when you're done with it so other clients can use it.
    virtual bool IsOpen();
        ///<determines whether or not the audio player is open
        ///
        /// %IsOpen() determines whether or not the audio player is open by
        /// this instance.
        ///
        /// @return whether or not the audio player is open
    enum { kOutputFlagAnalog = (1 << 0), kOutputFlagSPDIF = (1 << 1) };    
    virtual bool QueueStream(CascadeStream & stream, const char * pType = NULL, u32 nOutputFlags = (kOutputFlagAnalog|kOutputFlagSPDIF));
        ///<queues a stream for playback
        ///
        /// %QueueStream() queues a stream for playback, returning a unique id for the queued stream.
        /// A major use of queueing is to queue a second stream before the first one is finished
        /// to allow seamless playback transitioning.  NOTE: if the sample rate changes
        /// between streams, then a short delay will take place between streams.
        ///
        /// Once a stream has finished playing back, it is removed from the queue.
        /// and OnPlaybackComplete() is called with the completion status.
        ///
        /// From the time a stream is queued, it is owned by a distinct CascadeAudioPlayer thread
        /// and must not be used by the client outside of client notification functions
        /// until %OnPlaybackEnd() is called at which point the ownership of the stream is returned
        /// to the client.
        ///
        /// The client may optionally pass in a stream type into this function.  If a stream
        /// type is passed in, the codec registered for that type will be used, if any.
        /// If no codec is found registered to that type, then playback will fail and your
        /// OnAudioPlaybackEnd() function will be called to return the stream to you immediately.
        /// If NULL or "" is passed in as the type, then auto-select will be engaged and CascadeAudioPlayer
        /// will quiz all registered codecs and if it finds one that can handle the stream (based
        /// on examination of stream data), it will use it.
        ///
        /// The following values for pType are envisioned to be supported:<pre>
        ///  NULL           - auto select the type if possible
        ///  ""             - auto select the type if possible
        ///  "MP3"          - an MP3 stream
        ///  "AAC"          - an AAC stream
        ///  "WAV"          - an uncompressed WAV stream
        ///  "AIFF"         - an uncompressed AIFF stream
        //   "PCM:samplerate:numchannels:bitspersample:polarity:endianness:order:durationinmilliseconds"
        ///                 - a raw PCM sample stream
        ///                   e.g. "PCM:44100:2:16:S:L:LR:0"
        /// </pre>
        /// Currently, the following stream types are <b>actually</b> supported:<pre>
        ///  "MP3"          - an MP3 stream
        ///  "AAC"          - an AAC stream
        ///  "WAV"          - an uncompressed WAV stream
        ///  "AIFF"         - an uncompressed AIFF stream
        ///  "PCM:..."      - a raw PCM sample stream as described above
        /// </pre>
        /// Note that auto-select is not yet implemented.
        ///
        /// @param stream the stream to queue
        /// @param pType the type of the stream
        /// @return whether or not queuing succeeded
        /// @see CascadeAudioPlayer::Client::OnPlaybackBegin()
        /// @see CascadeAudioPlayer::Client::OnPlaybackEnd()
    virtual bool Stop();
        ///<stops playback and purges all queued streams
        ///
        /// %Stop() stops playback immediately and removes any queued
        /// streams from the queue.  No further notification functions are called.
        /// @return whether stopping succeeded
    virtual bool Pause();
        ///<pauses playback
        ///
        /// %Pause() puts the player into a paused state.
        /// @return whether pausing succeeded
    virtual bool Resume();
        ///<resumes playback
        ///
        /// %Resume() resumes playback of a previously paused stream
        /// @return whether playback resumption succeeded
	virtual bool SetVolume(u16 nVolume);
        ///<sets the playback volume
        ///
        /// %SetVolume() sets the output volume of audio playback.  Legal values for
        /// nVolume are 0 - 0xFFFF which is a linear range of volume
        /// where 0 is no volume and 0xFFFF is full volume.
        /// @param nVolume the volume to set
        /// @return whether the volume change succeeded
	virtual bool SetVolumePercent(u8 nVolume);
        ///<sets the playback volume in percent, on a logarithmic scale
        ///
        /// %SetVolumePercent() sets the output volume of audio playback.  
        /// Legal values for nVolume are 0 - 100, where 0 is -infinity dB,
        /// 1 is -30dB, and 100 is 0dB.  The values in between 1 and 100 are
        /// on a logarithmic scale to be pleasant to human hearing.
        /// @param nVolume the volume to set
        /// @return whether the volume change succeeded
    virtual bool IsPlaying();
        ///<determines whether or not the file is playing
        ///
        /// %IsPlaying() determines whether or not the mp3 file is playing.
        /// @return whether or not the file is playing
    virtual bool IsStopped();
        ///<determines whether or not the file is stopped
        ///
        /// %IsStopped() determines whether or not the mp3 file is stopped.
        /// @return whether or not the file is stopped
    virtual bool IsPaused();
        ///<determines whether or not the file is paused
        ///
        /// %IsPaused() determines whether or not the mp3 file is paused.
        /// @return whether or not the file is paused
    virtual bool AttachDevice(CascadeAudioPlaybackDevice & device);
        ///<attaches a custom playback device (not required)
        ///
        /// %AttachDevice() may be used to attach a custom (non-default)
        /// audio playback device for use by the player.
        ///
        /// If no device is ever attached with this function, the default system playback device
        /// is used.
        ///
        /// The player must be in the stopped state for this function to succeed.
        ///
        /// @param device the device to attach
        /// @return whether or not the attachment was successful
    virtual bool DetachDevice();
        ///<detaches a previously attached custom playback device
        ///
        /// %DetachDevice() detaches a previously attached custom playback device restoring
        /// the player to using the default audio playback device.
        ///
        /// The player must be in the stopped state for this function to succeed.
        ///
        /// @return whether or not the detachment was successful
public:    
    class SampleSet
    {
    public:
        u32  m_nStreamID;               ///<unique ID indicating which stream this sample set is from
        u32  m_nSequenceNumber;         ///<starts at 0 in each stream, increments by 1 each time OnVisualizePrepare is called
        u32  m_nNumSamples;             ///<indicates the number of samples in the set
        u8 * m_pSamples;                ///<pointer to the samples
        u32  m_nTimestampMilliseconds;  ///<number of milliseconds into the audio at the start of the sample set
        u32  m_nDurationMilliseconds;   ///<duration of the samples in the set; computed so you don't have to
        u32  m_nSampleBytes;            ///<indicates the number of bytes m_pSamples points to; computed so you don't have to
    public:
        u32 m_nNumChannels;             ///<number of channels of sample data
        u32 m_nSampleRate;              ///<sample rate
        u32 m_nBitsPerSample;           ///<number of bits per sample
    };
        /// @class SampleSet represents set of samples for visualization
        ///
        /// %SampleSet contains all the data necessary to perform audio visualizations of any type.
        /// %SampleSets are passed into %OnVisualizePrepareDT() to allow clients to generate visualization
        /// data from the sample set.
        ///
        /// @see CascadeAudioPlayer::Client::OnVisualizePrepareDT()  

    enum PlaybackResult
    { 
        kNormalCompletion = 0,              ///<indicates normal completion
        kPlaybackStopped,                   /// 1 <indicates Stop() was called
        kErrorUnknown,                      /// 2 <indicates an unknown error occurred
        kErrorOutOfMemory,                  /// 3 <indicates an out of memory error occurred
        kErrorUnsupportedSampleRate,        /// 4 <indicates an unsupported sample rate
        kErrorUnsupportedBitsPerSample,     /// 5 <indicates an unsupported number of bits per sample
        kErrorUnsupportedNumChannels,       /// 6 <indicates an unsupported number of channels
        kErrorUnsupportedSamplePolarity,    /// 7 <indicates an unsupported sample polarity (signed vs. unsigned)
        kErrorUnsupportedSampleEndianness,  /// 8 <indicates an unsupported sample endianness (little vs. big)
        kErrorUnsupportedSampleOrder,       /// 9 <indicates an unsupported sample order (leftRight vs. rightLeft)
        kErrorUnknownAudioType,             ///10 <indicates an inability to determine audio input format
        kErrorCodecInputFailure,            ///11 <indicates codec reported an error setting the input stream
        kErrorStreamReadFailure,            ///12 <indicates an error reading from the stream
        kErrorDeviceNonexistant,            ///13 <indicates an absence of any playback devices
        kErrorDeviceFailure,                ///14 <indicates a device error failure
        kErrorDeviceUnavailable,            ///15 <indicates the device is unavailable (already open by someone else)
        kErrorUnsupportedOutputFlags,       ///16 <indicates the device did not accept the output flags passed into %QueueStream()
    };
        /// @enum PlaybackResult represents a type of playback result
        ///
        /// %PlaybackResult represents the types of playback results that can occur.
        /// A variable of this type is provided to the virtual function %OnPlaybackEndDT()
        /// to indicate the result of the playback.
        ///
        /// @see CascadeAudioPlayer::Client::OnAudioPlaybackEndDT()  
    
    static const char * PlaybackResultToString(PlaybackResult result);

    class Client
    {
    public:
        virtual void OnAudioDecodeBegin(u32 nStreamID, CascadeStream * pStream, u32 nDurationInMilliseconds);
            // called when decoding of the stream begins
            // NOTE the nStreamID which is assigned to the stream indicated by pStream.
            // You'll need to NOTE it because that is what subsequent notifications use to uniquely identify the stream.
        virtual void OnAudioBuffering(u32 nStreamID, u32 nBuffersFilled, u32 nBuffersToFill);
            // Indicates that buffering is occurring either becuase of an initial pre-load or because
            // an underflow occurred.  nBuffersFilled / nBuffersToFill * 100 is the percent complete
            // of the buffering phase.  OnAudioBuffering gets called throughout the buffering
            // phase until nBuffersFilled == nBuffersToFill.
        virtual void OnAudioPlaybackBegin(u32 nStreamID);
            // Indicates audio has started to come out.  This function is called after pre-loading of buffers
            // is complete and audio starts actually playing, and also after an underflow when audio-buffering
            // has been re-engaged and finished, and audio is again flowing.
        virtual void OnAudioDecodeEnd(u32 nStreamID, CascadeStream * pStream, PlaybackResult result, u32 nPlaybackResultData);
            // Indicates that the player has finished decoding (and using) pStream.
            // pStream is now owned by the client again (CascadeAudioPlayer assumed ownership when QueueStream() was
            // called).  The client may feel free to delete pStream if they wish.
            // Note that OnAudioDecodeEnd() may be called significantly in advance of the end of the actual
            // audio playback.
        virtual void OnAudioPlaybackEnd(u32 nStreamID, PlaybackResult result);
            // Called when audio has finished playing.

        virtual void OnVisualizePrepare(u32 nStreamID, const SampleSet & samples);
            ///<called in the context of a distinct thread to allow clients to generate visualization data
            ///
            /// %OnVisualizePrepare() is called in the context of a distinct thread as soon as
            /// a set of samples is ready, before the samples are queued to the device for playback.
            /// Use this function to generate visualization data, but don't visualize it
            /// until OnVisualizeExecute() is called later.
            ///
            /// You should do all of your visualization data preparation in this function.  After this function
            /// returns the sample data in the sample set passed in is invalid.
            ///
            /// @param samples the sample set just decoded
            /// @note Any processing you do in this function will add delay to the decoding cycle.
            /// Normally this will not pose a problem as the buffer sizes selected internally are
            /// tailored to allow for visualization generation to occur in-step with the decoding process.
            /// Note, however that if you introduce too much delay, the audio device will underflow with data.
        virtual void OnVisualizeExecute(u32 nStreamID, u32 nSequenceNumber, u32 nTimestampMilliseconds, u32 nBuffersQueued, u32 nTotalBuffers);
            ///<called in the context of a distinct thread to notify clients when data should be visualized
            ///
            /// %OnVisualizeExecute() is called in the context of a distnct thread at the moment
            /// the audio device actually plays the first sample of a sample set previously passed in
            /// to %OnVisualizePrepare().  The sample set the notification applies to is the set
            /// that had the same sequence number and timestamp as those passed in.  In addition, the
            /// state of buffer fullness is given in terms of the number of buffers filled and queued
            /// in the driver out of a number of total buffers in the driver chain.
            /// So if nBuffersQueued == 4, and nTotalBuffers == 8, then the driver has 50% buffer fullness. 
            ///
            /// You should perform the actual visualization of the data you previously prepared,
            /// either in the context of this distinct thread, or by signalling your own thread to do so.
            ///
            /// @param nStreamUniqueID the unique stream ID of the sample set the notification applies to
            /// @param nTimestampMilliseconds the timestamp of the sample set the notification applies to
            /// @param nBuffersQueued the current number of buffers in the driver chain queued
            /// @param nTotalBuffers the total number of buffers in the driver chain 
            /// @note Like %OnVisualizePrepare(), any processing you do in this function will add
            /// delay to the decoding cycle.  If too much delay is introduced, the audio device will underflow with data.
            /// @note It is possible that more than one driver buffer will have played between invocations
            /// of this function (because decoding took longer than 1 buffer took to play).  In this event,
            /// you will only be called back with the most recently started timestamp, i.e. notifications
            /// can be lost.
        virtual void OnPlaybackPause();
        virtual void OnPlaybackResume();
        virtual void OnPlaybackStop();
    };
#ifndef DOXY_SKIP
    private:
        friend class CascadePrivate::CascadeAudioPlayerInitializer;
#endif
private:
    class Player;
    Player * m_pPlayer;
};

#endif // #ifndef _ROKU_INCLUDE_CASCADE_AV_CASCADEAUDIOPLAYER_H

///////////////////////////////////////////////////////////////////////////////
//  LOG
///////////////////////////////////////////////////////////////////////////////
//  07-Mar-04	dwoodward	created from CascadeMP3Player
//  29-Mar-04   dwoodward   added codec registration
//  25-May-04   mjkobb      Updated a comment
//  16-Jun-04   dsletten    added nStreamUniqueID to SampleSet and OnVisualizeExecute()
//  23-Jun-04   dwoodward   added Open() and Close()
//  29-Sep-04   dwoodward   Added nOutputFlags parameter to QueueStream()
//  09-Dec-04   mjkobb      Added SetVolumePercent
//  28-Mar-05   dwoodward   got rid of RegisterCodec
//  15-Apr-05   dwoodward   added OnPlaybackPause(), OnPlaybackResume(), OnPlaybackStop(), PlaybackResultToString()
