#pragma once

#include "Core.hpp"

/// TODO: finish. Will finish after FPacket
namespace MemoryPackets{
    /// @brief Memory Packet. Tries to be as memory efficient as possible holding the most data that it possible can.
    class MPacket{
    public:
        MPacket()noexcept{}
        MPacket(const HBuffer& data) : m_Buffer(data){}
        MPacket(HBuffer&& data) : m_Buffer(std::move(data)){}
    
    public:
        void WriteUInt8(uint8_t data)noexcept{
            
        }
    public:
        void SetReadPos(uint32_t readAt)noexcept{
            m_ReadPos = readAt;
        }
        void SetBuffer(const HBuffer& data)noexcept{
            m_Buffer = data;
        }
        void SetBuffer(HBuffer&& data)noexcept{
            m_Buffer = std::move(data);
        }
    public:
        HBuffer& GetBuffer()const noexcept{return (HBuffer&)m_Buffer;}
    private:
        HBuffer m_Buffer;
        uint32_t m_ReadPos = 0;
    };
}