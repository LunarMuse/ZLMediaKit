﻿/*
 * Copyright (c) 2016-present The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/ZLMediaKit/ZLMediaKit).
 *
 * Use of this source code is governed by MIT-like license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#ifndef ZLMEDIAKIT_FRAME_H
#define ZLMEDIAKIT_FRAME_H

#include <map>
#include <mutex>
#include <functional>
#include "Util/List.h"
#include "Util/TimeTicker.h"
#include "Common/Stamp.h"
#include "Network/Buffer.h"

namespace mediakit {

class Stamp;

typedef enum {
    TrackInvalid = -1,
    TrackVideo = 0,
    TrackAudio,
    TrackTitle,
    TrackApplication,
    TrackMax
} TrackType;

#define CODEC_MAP(XX) \
    XX(CodecH264,  TrackVideo, 0, "H264", PSI_STREAM_H264, MOV_OBJECT_H264)          \
    XX(CodecH265,  TrackVideo, 1, "H265", PSI_STREAM_H265, MOV_OBJECT_HEVC)          \
    XX(CodecAAC,   TrackAudio, 2, "mpeg4-generic", PSI_STREAM_AAC, MOV_OBJECT_AAC)   \
    XX(CodecG711A, TrackAudio, 3, "PCMA", PSI_STREAM_AUDIO_G711A, MOV_OBJECT_G711a)  \
    XX(CodecG711U, TrackAudio, 4, "PCMU", PSI_STREAM_AUDIO_G711U, MOV_OBJECT_G711u)  \
    XX(CodecOpus,  TrackAudio, 5, "opus", PSI_STREAM_AUDIO_OPUS, MOV_OBJECT_OPUS)    \
    XX(CodecL16,   TrackAudio, 6, "L16", PSI_STREAM_RESERVED, MOV_OBJECT_NONE)       \
    XX(CodecVP8,   TrackVideo, 7, "VP8", PSI_STREAM_VP8, MOV_OBJECT_VP8)             \
    XX(CodecVP9,   TrackVideo, 8, "VP9", PSI_STREAM_VP9, MOV_OBJECT_VP9)             \
    XX(CodecAV1,   TrackVideo, 9, "AV1", PSI_STREAM_AV1, MOV_OBJECT_AV1)             \
    XX(CodecJPEG,  TrackVideo, 10, "JPEG", PSI_STREAM_JPEG_2000, MOV_OBJECT_JPEG)    \
    XX(CodecH266,  TrackVideo, 11, "H266", PSI_STREAM_H266, MOV_OBJECT_H266)         \
    XX(CodecTS,    TrackVideo, 12, "MP2T", PSI_STREAM_RESERVED, MOV_OBJECT_NONE)     \
    XX(CodecPS,    TrackVideo, 13, "MPEG", PSI_STREAM_RESERVED, MOV_OBJECT_NONE)     \
    XX(CodecMP3,   TrackAudio, 14, "MP3",  PSI_STREAM_MP3, MOV_OBJECT_MP3)           \
    XX(CodecADPCM, TrackAudio, 15, "ADPCM", PSI_STREAM_RESERVED, MOV_OBJECT_NONE)    \
    XX(CodecSVACV, TrackVideo, 16, "SVACV", PSI_STREAM_VIDEO_SVAC, MOV_OBJECT_NONE)  \
    XX(CodecSVACA, TrackAudio, 17, "SVACA", PSI_STREAM_AUDIO_SVAC, MOV_OBJECT_NONE)  \
    XX(CodecG722,  TrackAudio, 18, "G722", PSI_STREAM_AUDIO_G722, MOV_OBJECT_NONE)   \
    XX(CodecG723,  TrackAudio, 19, "G723", PSI_STREAM_AUDIO_G723, MOV_OBJECT_NONE)   \
    XX(CodecG728,  TrackAudio, 20, "G728", PSI_STREAM_RESERVED, MOV_OBJECT_NONE)     \
    XX(CodecG729,  TrackAudio, 21, "G729", PSI_STREAM_AUDIO_G729, MOV_OBJECT_NONE)

typedef enum {
    CodecInvalid = -1,
#define XX(name, type, value, str, mpeg_id, mp4_id) name = value,
    CODEC_MAP(XX)
#undef XX
    CodecMax
} CodecId;

/**
 * 字符串转媒体类型转
 * String to media type conversion
 
 * [AUTO-TRANSLATED:59850011]
 */
TrackType getTrackType(const std::string &str);

/**
 * 媒体类型转字符串
 * Media type to string conversion
 
 * [AUTO-TRANSLATED:0456e0e2]
 */
const char* getTrackString(TrackType type);

/**
 * 根据SDP中描述获取codec_id
 * @param str
 * @return
 * Get codec_id from SDP description
 * @param str
 * @return
 
 * [AUTO-TRANSLATED:024f2ed1]
 */
CodecId getCodecId(const std::string &str);

/**
 * 获取编码器名称
 * Get encoder name
 
 * [AUTO-TRANSLATED:0253534b]
 */
const char *getCodecName(CodecId codecId);

/**
 * 获取音视频类型
 * Get audio/video type
 
 * [AUTO-TRANSLATED:e2f06ac2]
 */
TrackType getTrackType(CodecId codecId);

/**
 * 根据codecid获取mov object id
 * Get mov object id by codecid
 
 * [AUTO-TRANSLATED:c315b87d]
 */
int getMovIdByCodec(CodecId codecId);

/**
 * 根据mov object id获取CodecId
 * Get CodecId by mov object id
 
 * [AUTO-TRANSLATED:de2237a1]
 */
CodecId getCodecByMovId(int object_id);

/**
 * 根据codecid获取mpeg id
 * Get mpeg id by codecid
 
 * [AUTO-TRANSLATED:d365eac7]
 */
int getMpegIdByCodec(CodecId codec);

/**
 * 根据mpeg id获取CodecId
 * Get CodecId by mpeg id
 
 * [AUTO-TRANSLATED:ca190565]
 */
CodecId getCodecByMpegId(int mpeg_id);

/**
 * 编码信息的抽象接口
 * Abstract interface for encoding information
 
 * [AUTO-TRANSLATED:c3b14625]
 */
class CodecInfo {
public:
    using Ptr = std::shared_ptr<CodecInfo>;

    virtual ~CodecInfo() = default;

    /**
     * 获取编解码器类型
     * Get codec type
     
     * [AUTO-TRANSLATED:2dbf103b]
     */
    virtual CodecId getCodecId() const = 0;

    /**
     * 获取编码器名称
     * Get encoder name
     
     * [AUTO-TRANSLATED:a92f41f6]
     */
    const char *getCodecName() const;

    /**
     * 获取音视频类型
     * Get audio/video type
     
     * [AUTO-TRANSLATED:ff8ba95b]
     */
    TrackType getTrackType() const;

    /**
     * 获取音视频类型描述
     * Get audio/video type description
     
     * [AUTO-TRANSLATED:a460e432]
     */
    std::string getTrackTypeStr() const;

    /**
     * 设置track index, 用于支持多track
     * Set track index, for multi-track support
     
     * [AUTO-TRANSLATED:da5bdb91]
     */
    void setIndex(int index) { _index = index; }

    /**
     * 获取track index, 用于支持多track
     * Get track index, for multi-track support
     
     * [AUTO-TRANSLATED:1e96c587]
     */
    int getIndex() const { return _index < 0 ? (int)getTrackType() : _index; }

private:
    int _index = -1;
};

/**
 * 帧类型的抽象接口
 * Abstract interface for frame types
 
 * [AUTO-TRANSLATED:eb166e7e]
 */
class Frame : public toolkit::Buffer, public CodecInfo {
public:
    using Ptr = std::shared_ptr<Frame>;

    /**
     * 返回解码时间戳，单位毫秒
     * Return decoding timestamp, in milliseconds
     
     * [AUTO-TRANSLATED:00072dad]
     */
    virtual uint64_t dts() const = 0;

    /**
     * 返回显示时间戳，单位毫秒
     * Return display timestamp, in milliseconds
     
     * [AUTO-TRANSLATED:c7eecb91]
     */
    virtual uint64_t pts() const { return dts(); }

    /**
     * 前缀长度，譬如264前缀为0x00 00 00 01,那么前缀长度就是4
     * aac前缀则为7个字节
     * Prefix length, for example, the 264 prefix is 0x00 00 00 01, so the prefix length is 4
     * aac prefix is 7 bytes
     
     * [AUTO-TRANSLATED:6334f58e]
     */
    virtual size_t prefixSize() const = 0;

    /**
     * 返回是否为关键帧
     * Return whether it is a key frame
     
     * [AUTO-TRANSLATED:2e52426a]
     */
    virtual bool keyFrame() const = 0;

    /**
     * 是否为配置帧，譬如sps pps vps
     * Whether it is a configuration frame, such as sps pps vps
     
     * [AUTO-TRANSLATED:595c7ecf]
     */
    virtual bool configFrame() const = 0;

    /**
     * 是否可以缓存
     * Whether it can be cached
     
     * [AUTO-TRANSLATED:5c35d3e0]
     */
    virtual bool cacheAble() const { return true; }

    /**
     * 该帧是否可以丢弃
     * SEI/AUD帧可以丢弃
     * 默认都不能丢帧
     * Whether this frame can be dropped
     * SEI/AUD frames can be dropped
     * By default, no frames can be dropped
     
     * [AUTO-TRANSLATED:42957087]
     */
    virtual bool dropAble() const { return false; }

    /**
     * 是否为可解码帧
     * sps pps等帧不能解码
     * Whether it is a decodable frame
     * sps pps frames cannot be decoded
     
     * [AUTO-TRANSLATED:52f093c7]
     */
    virtual bool decodeAble() const {
        if (getTrackType() != TrackVideo) {
            // 非视频帧都可以解码  [AUTO-TRANSLATED:24aa4342]
            // Non-video frames can be decoded
            return true;
        }
        // 默认非sps pps帧都可以解码  [AUTO-TRANSLATED:b14d1e34]
        // By default, non-sps pps frames can be decoded
        return !configFrame();
    }

    /**
     * 返回可缓存的frame
     * Return the cacheable frame
     
     * [AUTO-TRANSLATED:88fb9c3e]
     */
    static Ptr getCacheAbleFrame(const Ptr &frame);

private:
    // 对象个数统计  [AUTO-TRANSLATED:3b43e8c2]
    // Object count statistics
    toolkit::ObjectStatistic<Frame> _statistic;
};

class FrameImp : public Frame {
public:
    using Ptr = std::shared_ptr<FrameImp>;

    template <typename C = FrameImp>
    static std::shared_ptr<C> create() {
#if 0
        static ResourcePool<C> packet_pool;
        static onceToken token([]() {
            packet_pool.setSize(1024);
        });
        auto ret = packet_pool.obtain2();
        ret->_buffer.clear();
        ret->_prefix_size = 0;
        ret->_dts = 0;
        ret->_pts = 0;
        return ret;
#else
        return std::shared_ptr<C>(new C());
#endif
    }

    char *data() const override { return (char *)_buffer.data(); }
    size_t size() const override { return _buffer.size(); }
    uint64_t dts() const override { return _dts; }
    uint64_t pts() const override { return _pts ? _pts : _dts; }
    size_t prefixSize() const override { return _prefix_size; }
    CodecId getCodecId() const override { return _codec_id; }
    bool keyFrame() const override { return false; }
    bool configFrame() const override { return false; }

public:
    CodecId _codec_id = CodecInvalid;
    uint64_t _dts = 0;
    uint64_t _pts = 0;
    size_t _prefix_size = 0;
    toolkit::BufferLikeString _buffer;

private:
    // 对象个数统计  [AUTO-TRANSLATED:3b43e8c2]
    // Object count statistics
    toolkit::ObjectStatistic<FrameImp> _statistic;

protected:
    friend class toolkit::ResourcePool_l<FrameImp>;
    FrameImp() = default;
};

// 包装一个指针成不可缓存的frame  [AUTO-TRANSLATED:c3e5d65e]
// Wrap a pointer into a non-cacheable frame
class FrameFromPtr : public Frame {
public:
    using Ptr = std::shared_ptr<FrameFromPtr>;

    FrameFromPtr(CodecId codec_id, char *ptr, size_t size, uint64_t dts, uint64_t pts = 0, size_t prefix_size = 0, bool is_key = false)
        : FrameFromPtr(ptr, size, dts, pts, prefix_size, is_key) {
        _codec_id = codec_id;
    }

    char *data() const override { return _ptr; }
    size_t size() const override { return _size; }
    uint64_t dts() const override { return _dts; }
    uint64_t pts() const override { return _pts ? _pts : dts(); }
    size_t prefixSize() const override { return _prefix_size; }
    bool cacheAble() const override { return false; }
    bool keyFrame() const override { return _is_key; }
    bool configFrame() const override { return false; }

    CodecId getCodecId() const override {
        if (_codec_id == CodecInvalid) {
            throw std::invalid_argument("Invalid codec type of FrameFromPtr");
        }
        return _codec_id;
    }

protected:
    FrameFromPtr() = default;

    FrameFromPtr(char *ptr, size_t size, uint64_t dts, uint64_t pts = 0, size_t prefix_size = 0, bool is_key = false) {
        _ptr = ptr;
        _size = size;
        _dts = dts;
        _pts = pts;
        _prefix_size = prefix_size;
        _is_key = is_key;
    }

protected:
    bool _is_key;
    char *_ptr;
    uint64_t _dts;
    uint64_t _pts = 0;
    size_t _size;
    size_t _prefix_size;
    CodecId _codec_id = CodecInvalid;
};

/**
 * 一个Frame类中可以有多个帧(AAC)，时间戳会变化
 * ZLMediaKit会先把这种复合帧split成单个帧然后再处理
 * 一个复合帧可以通过无内存拷贝的方式切割成多个子Frame
 * 提供该类的目的是切割复合帧时防止内存拷贝，提高性能
 * A Frame class can have multiple frames (AAC), and the timestamp will change
 * ZLMediaKit will first split this composite frame into single frames and then process it
 * A composite frame can be split into multiple sub-Frames without memory copy
 * The purpose of providing this class is to prevent memory copy when splitting composite frames, improving performance
 
 * [AUTO-TRANSLATED:4010c0a5]
 */
template <typename Parent>
class FrameInternalBase : public Parent {
public:
    using Ptr = std::shared_ptr<FrameInternalBase>;
    FrameInternalBase(Frame::Ptr parent_frame, char *ptr, size_t size, uint64_t dts, uint64_t pts = 0, size_t prefix_size = 0)
        : Parent(parent_frame->getCodecId(), ptr, size, dts, pts, prefix_size) {
        _parent_frame = std::move(parent_frame);
        this->setIndex(_parent_frame->getIndex());
    }

    bool cacheAble() const override { return _parent_frame->cacheAble(); }

private:
    Frame::Ptr _parent_frame;
};

/**
 * 一个Frame类中可以有多个帧，他们通过 0x 00 00 01 分隔
 * ZLMediaKit会先把这种复合帧split成单个帧然后再处理
 * 一个复合帧可以通过无内存拷贝的方式切割成多个子Frame
 * 提供该类的目的是切割复合帧时防止内存拷贝，提高性能
 * A Frame class can have multiple frames, they are separated by 0x 00 00 01
 * ZLMediaKit will first split this composite frame into single frames and then process it
 * A composite frame can be split into multiple sub-Frames without memory copy
 * The purpose of providing this class is to prevent memory copy when splitting composite frames, improving performance
 
 * [AUTO-TRANSLATED:ed49148b]
 */
template <typename Parent>
class FrameInternal : public FrameInternalBase<Parent> {
public:
    using Ptr = std::shared_ptr<FrameInternal>;
    FrameInternal(const Frame::Ptr &parent_frame, char *ptr, size_t size, size_t prefix_size)
        : FrameInternalBase<Parent>(parent_frame, ptr, size, parent_frame->dts(), parent_frame->pts(), prefix_size) {}
};

// 管理一个指针生命周期并生产一个frame  [AUTO-TRANSLATED:449d107b]
// Manage the lifetime of a pointer and produce a frame
class FrameAutoDelete : public FrameFromPtr {
public:
    template <typename... ARGS>
    FrameAutoDelete(ARGS &&...args) : FrameFromPtr(std::forward<ARGS>(args)...) {}

    ~FrameAutoDelete() override { delete[] _ptr; };

    bool cacheAble() const override { return true; }
};

// 把一个不可缓存的frame声明为可缓存的  [AUTO-TRANSLATED:2c8d0659]
// Declare a non-cacheable frame as cacheable
template <typename Parent>
class FrameToCache : public Parent {
public:
    template<typename ... ARGS>
    FrameToCache(ARGS &&...args) : Parent(std::forward<ARGS>(args)...) {};

    bool cacheAble() const override {
        return true;
    }
};

// 该对象的功能是把一个不可缓存的帧转换成可缓存的帧  [AUTO-TRANSLATED:5851119b]
// The function of this object is to convert a non-cacheable frame into a cacheable frame
class FrameCacheAble : public FrameFromPtr {
public:
    using Ptr = std::shared_ptr<FrameCacheAble>;

    FrameCacheAble(const Frame::Ptr &frame, bool force_key_frame = false, toolkit::Buffer::Ptr buf = nullptr) {
        setIndex(frame->getIndex());
        if (frame->cacheAble()) {
            _ptr = frame->data();
            _buffer = frame;
        } else if (buf) {
            _ptr = frame->data();
            _buffer = std::move(buf);
        } else {
            auto buffer = std::make_shared<toolkit::BufferLikeString>();
            buffer->assign(frame->data(), frame->size());
            _ptr = buffer->data();
            _buffer = std::move(buffer);
        }
        _size = frame->size();
        _dts = frame->dts();
        _pts = frame->pts();
        _prefix_size = frame->prefixSize();
        _codec_id = frame->getCodecId();
        _key = force_key_frame ? true : frame->keyFrame();
        _config = frame->configFrame();
        _drop_able = frame->dropAble();
        _decode_able = frame->decodeAble();
    }

    /**
     * 可以被缓存
     * Can be cached
     
     * [AUTO-TRANSLATED:7f9cec13]
     */
    bool cacheAble() const override { return true; }
    bool keyFrame() const override { return _key; }
    bool configFrame() const override { return _config; }
    bool dropAble() const override { return _drop_able; }
    bool decodeAble() const override { return _decode_able; }

private:
    bool _key;
    bool _config;
    bool _drop_able;
    bool _decode_able;
    toolkit::Buffer::Ptr _buffer;
};

// 该类实现frame级别的时间戳覆盖  [AUTO-TRANSLATED:77c28d0f]
// This class implements frame-level timestamp overwrite
class FrameStamp : public Frame {
public:
    using Ptr = std::shared_ptr<FrameStamp>;
    FrameStamp(Frame::Ptr frame);
    FrameStamp(Frame::Ptr frame, Stamp &stamp, int modify_stamp);
    ~FrameStamp() override {}

    uint64_t dts() const override { return (uint64_t)_dts; }
    uint64_t pts() const override { return (uint64_t)_pts; }
    size_t prefixSize() const override { return _frame->prefixSize(); }
    bool keyFrame() const override { return _frame->keyFrame(); }
    bool configFrame() const override { return _frame->configFrame(); }
    bool cacheAble() const override { return _frame->cacheAble(); }
    bool dropAble() const override { return _frame->dropAble(); }
    bool decodeAble() const override { return _frame->decodeAble(); }
    char *data() const override { return _frame->data(); }
    size_t size() const override { return _frame->size(); }
    CodecId getCodecId() const override { return _frame->getCodecId(); }
    void setStamp(int64_t dts, int64_t pts);

private:
    int64_t _dts;
    int64_t _pts;
    Frame::Ptr _frame;
};

/**
 * 该对象可以把Buffer对象转换成可缓存的Frame对象
 * This object can convert a Buffer object into a cacheable Frame object
 
 * [AUTO-TRANSLATED:3c5786b8]
 */
template <typename Parent>
class FrameFromBuffer : public Parent {
public:
    /**
     * 构造frame
     * @param buf 数据缓存
     * @param dts 解码时间戳
     * @param pts 显示时间戳
     * @param prefix 帧前缀长度
     * @param offset buffer有效数据偏移量
     * Construct frame
     * @param buf Data cache
     * @param dts Decode timestamp
     * @param pts Display timestamp
     * @param prefix Frame prefix length
     * @param offset Buffer valid data offset
     
     * [AUTO-TRANSLATED:6afec0f1]
     */
    FrameFromBuffer(toolkit::Buffer::Ptr buf, uint64_t dts, uint64_t pts, size_t prefix = 0, size_t offset = 0)
        : Parent(buf->data() + offset, buf->size() - offset, dts, pts, prefix) {
        _buf = std::move(buf);
    }

    /**
     * 构造frame
     * @param buf 数据缓存
     * @param dts 解码时间戳
     * @param pts 显示时间戳
     * @param prefix 帧前缀长度
     * @param offset buffer有效数据偏移量
     * @param codec 帧类型
     * Construct frame
     * @param buf Data cache
     * @param dts Decode timestamp
     * @param pts Display timestamp
     * @param prefix Frame prefix length
     * @param offset Buffer valid data offset
     * @param codec Frame type
     
     * [AUTO-TRANSLATED:f1c42e38]
     */
    FrameFromBuffer(CodecId codec, toolkit::Buffer::Ptr buf, uint64_t dts, uint64_t pts, size_t prefix = 0, size_t offset = 0)
        : Parent(codec, buf->data() + offset, buf->size() - offset, dts, pts, prefix) {
        _buf = std::move(buf);
    }

    /**
     * 该帧可缓存
     * This frame is cacheable
     
     * [AUTO-TRANSLATED:e089250f]
     */
    bool cacheAble() const override { return true; }

private:
    toolkit::Buffer::Ptr _buf;
};

/**
 * 合并一些时间戳相同的frame
 * Merge some frames with the same timestamp
 
 * [AUTO-TRANSLATED:392a23df]
 */
class FrameMerger {
public:
    using onOutput = std::function<void(uint64_t dts, uint64_t pts, const toolkit::Buffer::Ptr &buffer, bool have_key_frame)>;
    using Ptr = std::shared_ptr<FrameMerger>;
    enum {
        none = 0,
        h264_prefix,
        mp4_nal_size,
    };

    FrameMerger(int type);

    /**
     * 刷新输出缓冲，注意此时会调用FrameMerger::inputFrame传入的onOutput回调
     * 请注意回调捕获参数此时是否有效
     * Refresh the output buffer, note that FrameMerger::inputFrame's onOutput callback will be called at this time
     * Please note whether the callback capture parameters are valid at this time
     
     * [AUTO-TRANSLATED:18c25a14]
     */
    void flush();
    void clear();
    bool inputFrame(const Frame::Ptr &frame, onOutput cb, toolkit::BufferLikeString *buffer = nullptr);

private:
    bool willFlush(const Frame::Ptr &frame) const;
    void doMerge(toolkit::BufferLikeString &buffer, const Frame::Ptr &frame) const;

private:
    int _type;
    bool _have_decode_able_frame = false;
    bool _have_drop_able_frame = false;
    bool _have_config_frame = false;
    onOutput _cb;
    toolkit::List<Frame::Ptr> _frame_cache;
};

/**
 * 写帧接口的抽象接口类
 * Abstract interface class for write frame interface
 
 * [AUTO-TRANSLATED:dbe6a33c]
 */
class FrameWriterInterface {
public:
    using Ptr = std::shared_ptr<FrameWriterInterface>;
    virtual ~FrameWriterInterface() = default;

    /**
     * 写入帧数据
     * Write frame data
     
     * [AUTO-TRANSLATED:d46c6fc2]
     */
    virtual bool inputFrame(const Frame::Ptr &frame) = 0;

    /**
     * 刷新输出所有frame缓存
     * Flush all frame caches in the output
     
     * [AUTO-TRANSLATED:adaea568]
     */
    virtual void flush() {};
};

/**
 * 支持代理转发的帧环形缓存
 * Frame circular buffer that supports proxy forwarding
 
 * [AUTO-TRANSLATED:06bf1541]
 */
class FrameDispatcher : public FrameWriterInterface {
public:
    using Ptr = std::shared_ptr<FrameDispatcher>;

    /**
     * 添加代理
     * Add proxy
     
     * [AUTO-TRANSLATED:0be3c076]
     */
    FrameWriterInterface* addDelegate(FrameWriterInterface::Ptr delegate) {
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        return _delegates.emplace(delegate.get(), std::move(delegate)).first->second.get();
    }

    FrameWriterInterface* addDelegate(std::function<bool(const Frame::Ptr &frame)> cb);

    /**
     * 删除代理
     * Delete proxy
     
     * [AUTO-TRANSLATED:c2c915aa]
     */
    void delDelegate(FrameWriterInterface *ptr) {
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        _delegates.erase(ptr);
    }

    /**
     * 写入帧并派发
     * Write frame and dispatch
     
     * [AUTO-TRANSLATED:a3e7e6db]
     */
    bool inputFrame(const Frame::Ptr &frame) override {
        doStatistics(frame);
        bool ret = false;
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        for (auto &pr : _delegates) {
            if (pr.second->inputFrame(frame)) {
                ret = true;
            }
        }
        return ret;
    }

    /**
     * 返回代理个数
     * Return the number of proxies
     
     * [AUTO-TRANSLATED:93ebe7ec]
     */
    size_t size() const {
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        return _delegates.size();
    }

    void clear() {
        std::lock_guard<std::recursive_mutex> lck(_mtx);
        _delegates.clear();
    }

    /**
     * 获取累计关键帧数
     * Get the cumulative number of keyframes
     
     * [AUTO-TRANSLATED:73cb2ab0]
     */
    uint64_t getVideoKeyFrames() const {
        return _video_key_frames;
    }

    /**
     *  获取帧数
     * Get the number of frames
     
     * [AUTO-TRANSLATED:118b395e]
     */
    uint64_t getFrames() const {
        return _frames;
    }

    size_t getVideoGopSize() const {
        return _gop_size;
    }

    size_t getVideoGopInterval() const {
        return _gop_interval_ms;
    }

    int64_t getDuration() const {
        return _stamp.getRelativeStamp();
    }

private:
    void doStatistics(const Frame::Ptr &frame) {
        if (!frame->configFrame() && !frame->dropAble()) {
            // 忽略配置帧与可丢弃的帧  [AUTO-TRANSLATED:da4ff7ac]
            // Ignore configuration frames and discardable frames
            ++_frames;
            int64_t out;
            _stamp.revise(frame->dts(), frame->pts(), out, out);
            if (frame->keyFrame() && frame->getTrackType() == TrackVideo) {
                // 遇视频关键帧时统计  [AUTO-TRANSLATED:72b0e569]
                // Statistics when encountering video keyframes
                ++_video_key_frames;
                _gop_size = _frames - _last_frames;
                _gop_interval_ms = _ticker.elapsedTime();
                _last_frames = _frames;
                _ticker.resetTime();
            }
        }
    }

private:
    toolkit::Ticker _ticker;
    size_t _gop_interval_ms = 0;
    size_t _gop_size = 0;
    uint64_t _last_frames = 0;
    uint64_t _frames = 0;
    uint64_t _video_key_frames = 0;
    Stamp _stamp;
    mutable std::recursive_mutex _mtx;
    std::map<void *, FrameWriterInterface::Ptr> _delegates;
};

} // namespace mediakit
#endif // ZLMEDIAKIT_FRAME_H