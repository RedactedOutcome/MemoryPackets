#pragma once

#include "Core.hpp"

namespace MemoryPackets{
    /// @brief Memory Packet. Tries to be as memory efficient as possible holding the most data that it possible can.
    class FPacket{
    public:
        FPacket()noexcept{}
        FPacket(const HBuffer& data) : m_Buffer(data){}
        FPacket(HBuffer&& data) : m_Buffer(std::move(data)){}
    
    public:
    #pragma region Writing
        void WriteInt8(int8_t data)noexcept{
            m_Buffer.InsertInt8At(m_ReadPos, static_cast<int8_t>(data));
            m_ReadPos+=1;
        }
        void WriteUInt8(uint8_t data)noexcept{
            m_Buffer.InsertInt8At(m_ReadPos, static_cast<int8_t>(data));
            m_ReadPos+=1;
        }
        void WriteInt16(int16_t data)noexcept{
            m_Buffer.InsertInt16At(m_ReadPos, static_cast<int16_t>(data));
            m_ReadPos+=2;
        }
        void WriteUInt16(uint16_t data)noexcept{
            m_Buffer.InsertInt16At(m_ReadPos, static_cast<int16_t>(data));
            m_ReadPos+=2;
        }
        void WriteInt32(int32_t data)noexcept{
            m_Buffer.InsertInt32At(m_ReadPos, static_cast<int32_t>(data));
            m_ReadPos+=4;
        }
        void WriteUInt32(uint32_t data)noexcept{
            m_Buffer.InsertInt32At(m_ReadPos, static_cast<int32_t>(data));
            m_ReadPos+=4;
        }
    #pragma endregion
    public:
    #pragma region Reading
        /// @brief Reads an unsigned byte at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        uint8_t ReadUInt8(bool& status, bool moveReadPos)noexcept{
            uint8_t data = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return data;
            if(moveReadPos)m_ReadPos++;
            return data;
        }
        /// @brief Reads an byte at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        int8_t ReadInt8(bool& status, bool moveReadPos = true)noexcept{
            int8_t data = static_cast<int8_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return data;
            if(moveReadPos)m_ReadPos++;
            return data;
        }
        /// @brief Reads an uint16_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        uint16_t ReadUInt16(bool& status, bool moveReadPos = true)noexcept{
            uint16_t data = static_cast<uint16_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return data;
            if(moveReadPos)m_ReadPos+=2;
            return data;
        }
        /// @brief Reads an int16_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        int16_t ReadInt16(bool& status, bool moveReadPos = true)noexcept{
            int16_t data = static_cast<int16_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return data;
            if(moveReadPos)m_ReadPos+=2;
            return data;
        }
        
        /// @brief Reads an uint32_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        uint32_t ReadUInt32(bool& status, bool moveReadPos = true)noexcept{
            uint32_t data = static_cast<uint32_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return data;
            if(moveReadPos)m_ReadPos+=4;
            return data;
        }
        /// @brief Reads an int32_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        int32_t ReadInt32(bool& status, bool moveReadPos = true)noexcept{
            int32_t data = static_cast<int32_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return data;
            if(moveReadPos)m_ReadPos+=4;
            return data;
        }
    
        HBuffer ReadString(bool& status, bool moveReadPos = true, bool sizeEncoded = false)noexcept{
            HBuffer data;
            status = false;
            
            if(sizeEncoded){
                uint32_t size = ReadUInt32(status, false);
                data.ReserveString(size);
                size_t wasAt = m_ReadPos;
                m_ReadPos+=4;

                for(uint32_t i = 0; i < size; i++){
                    int8_t byte = ReadInt8(status);
                    if(!status){
                        if(!moveReadPos)m_ReadPos = wasAt;
                        return data;
                    }
                    data.Append(byte);
                }
                data.Append('\0');
                if(!moveReadPos)m_ReadPos = wasAt;

                status = true;
                return data;
            }
            int8_t byte;
            do{
                byte = ReadInt8(status);
                if(!status)return data;
                data.Append(static_cast<char>(byte));
            }while(byte != 0);

            status = true;
            return data;
        }
    #pragma endregion
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