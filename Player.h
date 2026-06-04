#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <expected>
#include <iostream>
#include <string>

#include "Renderer.h"

class Player {
   private:
    Renderer renderer;
    AVFormatContext* formatContext;
    AVCodecContext* codecContext;
    SwsContext* swsContext;
    AVFrame* scaledFrame;

    int videoStreamIndex, termWidth, termHeight;

   public:
    std::expected<bool, std::string> openVideo ( const std::string& filename ) {
        if ( avformat_open_input ( &formatContext, filename.c_str (), nullptr, nullptr ) < 0 ) {
            return std::unexpected ( "Failed to open the filename." );
        }
        if ( avformat_find_stream_info ( formatContext, nullptr ) < 0 ) {
            return std::unexpected ( "Failed to find stream info." );
        }
        return true;
    }

    std::expected<bool, std::string> setupDecoder () {
        videoStreamIndex = av_find_best_stream ( formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0 );

        if ( videoStreamIndex < 0 ) {
            return std::unexpected ( "Stream fail." );
        }

        const AVCodec* codec = avcodec_find_decoder ( formatContext->streams[ videoStreamIndex ]->codecpar->codec_id );
        if ( !codec ) {
            return std::unexpected ( "Codec not found." );
        }

        codecContext = avcodec_alloc_context3 ( codec );
        if ( !codecContext ) {
            return std::unexpected ( "Failed to allocate codec context." );
        }

        avcodec_parameters_to_context ( codecContext, formatContext->streams[ videoStreamIndex ]->codecpar );
        if ( avcodec_open2 ( codecContext, codec, nullptr ) < 0 ) {
            return std::unexpected ( "Failed to open codec." );
        }
        return true;
    }

    std::expected<bool, std::string> setupSizing () {
        swsContext = sws_getContext ( codecContext->width, codecContext->height, AV_PIX_FMT_YUV420P, termWidth,
                                      termHeight, AV_PIX_FMT_GRAY8, SWS_BILINEAR, nullptr, nullptr, nullptr );

        if ( !swsContext ) {
            return std::unexpected ( "Failed to create scaling context." );
        }

        scaledFrame = av_frame_alloc ();

        if ( !scaledFrame ) {
            return std::unexpected ( "Failed to allocate scaled frame." );
        }
        scaledFrame->width = termWidth;
        scaledFrame->height = termHeight;
        scaledFrame->format = AV_PIX_FMT_GRAY8;

        if ( av_frame_get_buffer ( scaledFrame, 0 ) < 0 ) {
            return std::unexpected ( "Failed to allocate frame buffer." );
        }

        return true;
    }

    void PlayVideo () {
        AVPacket* packet = av_packet_alloc ();
        AVFrame* frame = av_frame_alloc ();
        double fps = av_q2d ( formatContext->streams[ videoStreamIndex ]->avg_frame_rate );
        int delay = static_cast<int> ( 1000.0 / fps );

        std::cout << "Terminal: " << termWidth << "x" << termHeight << '\n';
        std::cout << "Scaled to: " << scaledFrame->width << "x" << scaledFrame->height << '\n';
        std::cin.get ();  // pause to read it

        system ( "pause" );

        while ( av_read_frame ( formatContext, packet ) >= 0 ) {
            if ( packet->stream_index == videoStreamIndex ) {
                avcodec_send_packet ( codecContext, packet );
                if ( avcodec_receive_frame ( codecContext, frame ) >= 0 ) {
                    renderer.clear ();
                    sws_scale ( swsContext, frame->data, frame->linesize, 0, codecContext->height, scaledFrame->data,
                                scaledFrame->linesize );
                    renderer.displayASCII ( scaledFrame->data[ 0 ], termWidth, scaledFrame->height,
                                            scaledFrame->linesize[ 0 ] );
                    std::this_thread::sleep_for ( std::chrono::milliseconds ( delay ) );
                }
            }
        }
    }

    Player ()
        : formatContext ( nullptr ),
          codecContext ( nullptr ),
          swsContext ( nullptr ),
          scaledFrame ( nullptr ),
          videoStreamIndex ( -1 ) {
        renderer.getTerminalSize ( termWidth, termHeight );
    }
};