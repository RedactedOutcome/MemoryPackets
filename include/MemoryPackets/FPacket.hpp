#pragma once

#include "Core.hpp"

namespace MemoryPackets{
    /// @brief Memory Packet. Tries to be as memory efficient as possible holding the most data that it possible can.
    class FPacket{
    public:
        FPacket()noexcept{}
        FPacket(const HBuffer& data) : m_Buffer(data){}
        FPacket(HBuffer&& data) : m_Buffer(std::move(data)){}
        FPacket(const HBuffer& data, uint32_t readPos) : m_Buffer(data), m_ReadPos(readPos){}
        FPacket(HBuffer&& data, uint32_t readPos) : m_Buffer(std::move(data)), m_ReadPos(readPos){}
    public:
        void SetReadPos(uint32_t pos)noexcept{
            m_ReadPos = pos;
        }
        void SetBuffer(const HBuffer& data)noexcept{
            m_Buffer = data;
        }
        void SetBuffer(HBuffer&& data)noexcept{
            m_Buffer = std::move(data);
        }
    public:
    #pragma region Writing
        void InsertLength()noexcept{
            HBuffer copy = m_Buffer.GetCopy();
            m_Buffer.InsertInt32At(0, m_ReadPos);
            m_Buffer.InsertAt(sizeof(uint32_t), copy);

            /// @brief sets to 0 since we are done with the packet and is now ready for reading
            m_ReadPos = 0;
        }
        /// @brief inserts the length of the buffer before all of the data
        void InsertLength(uint32_t length)noexcept{
            HBuffer copy = m_Buffer.GetCopy();
            m_Buffer.InsertInt32At(0, length);
            m_Buffer.InsertAt(sizeof(uint32_t), copy);
            
            /// @brief sets to 0 since we are done with the packet and is now ready for reading
            m_ReadPos=0;
        }
        void WriteLength() noexcept{
            m_Buffer.InsertInt32At(0, std::max((size_t)4, (size_t)m_ReadPos) - 4);
        }
        void WriteInt8(int8_t data)noexcept{
            m_Buffer.InsertInt8At(m_ReadPos, static_cast<int8_t>(data));
            m_ReadPos+=sizeof(int8_t);
        }
        void WriteUInt8(uint8_t data)noexcept{
            m_Buffer.InsertInt8At(m_ReadPos, static_cast<int8_t>(data));
            m_ReadPos+=sizeof(uint8_t);
        }
        void WriteInt16(int16_t data)noexcept{
            m_Buffer.InsertInt16At(m_ReadPos, static_cast<int16_t>(data));
            m_ReadPos+=sizeof(int16_t);
        }
        void WriteUInt16(uint16_t data)noexcept{
            m_Buffer.InsertInt16At(m_ReadPos, static_cast<int16_t>(data));
            m_ReadPos+=sizeof(uint16_t);
        }
        void WriteInt32(int32_t data)noexcept{
            m_Buffer.InsertInt32At(m_ReadPos, static_cast<int32_t>(data));
            m_ReadPos+=sizeof(int32_t);
        }
        void WriteUInt32(uint32_t data)noexcept{
            m_Buffer.InsertInt32At(m_ReadPos, static_cast<int32_t>(data));
            m_ReadPos+=sizeof(uint32_t);
        }

        /// @brief Writes a null terminated string into the buffer at the current read pos
        /// @param data must be a null terminated string. Gets appended to the current buffers read position
        void WriteString(const char* data)noexcept{
            size_t strLen = strlen(data);
            m_Buffer.InsertAt(m_ReadPos, HBuffer(const_cast<char*>(data), strLen, false, false));
            m_ReadPos+=strLen;
        }

        void WriteBuffer(const HBuffer& data)noexcept{
            m_Buffer.InsertInt32At(m_ReadPos, static_cast<int32_t>(data.GetSize()));
            m_Buffer.InsertAt(m_ReadPos + 4, data);
            m_ReadPos+=data.GetSize() + 4;
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
            uint8_t data1 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return 0;
            uint8_t data2 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 1));
            if(!status)return 0;
            if(moveReadPos)m_ReadPos+=sizeof(uint16_t);
            uint16_t data = 0;
        #if HBUFF_ENDIAN_MODE == 0
            return (data2 << 8) | data1;
        #else
            return (data1 << 24) | data2;
        #endif
        }
        /// @brief Reads an int16_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        int16_t ReadInt16(bool& status, bool moveReadPos = true)noexcept{
            uint8_t data1 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return 0;
            uint8_t data2 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 1));
            if(!status)return 0;
            if(moveReadPos)m_ReadPos+=sizeof(uint16_t);
            uint16_t data = 0;
        #if HBUFF_ENDIAN_MODE == 0
            return static_cast<int16_t>((data2 << 8) | data1);
        #else
            return static_cast<int16_t>((data1 << 24) | data2);
        #endif
        }
        
        /// @brief Reads an uint32_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        uint32_t ReadUInt32(bool& status, bool moveReadPos = true)noexcept{
            uint8_t data1 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return 0;
            uint8_t data2 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 1));
            if(!status)return 0;
            uint8_t data3 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 2));
            if(!status)return 0;
            uint8_t data4 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 3));
            if(!status)return 0;
            if(moveReadPos)m_ReadPos+=sizeof(uint32_t);
            uint32_t data = 0;
        #if HBUFF_ENDIAN_MODE == 0
            return (data4 << 24) | (data3 << 16) | (data2 << 8) | data1;
        #else
            return (data1 << 24) | (data2 << 16) | (data3 << 8) | data4;
        #endif
        }
        /// @brief Reads an int32_t at the current Read position.
        /// @param moveReadPos decides if we want to move the read at position inside the buffer when we are done
        /// @return returns the byte if we are able to read else undefined and status is false
        int32_t ReadInt32(bool& status, bool moveReadPos = true)noexcept{
            uint8_t data1 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos));
            if(!status)return 0;
            uint8_t data2 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 1));
            if(!status)return 0;
            uint8_t data3 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 2));
            if(!status)return 0;
            uint8_t data4 = static_cast<uint8_t>(m_Buffer.Retrieve(status, m_ReadPos + 3));
            if(!status)return 0;
            if(moveReadPos)m_ReadPos+=sizeof(int32_t);
            uint32_t data = 0;
        #if HBUFF_ENDIAN_MODE == 0
            return static_cast<int32_t>((data4 << 24) | (data3 << 16) | (data2 << 8) | data1);
        #else
            return static_cast<int32_t>((data1 << 24) | (data2 << 16) | (data3 << 8) | data4);
        #endif
        }
    
        HBuffer ReadString(bool& status, bool moveReadPos = true, bool sizeEncoded = false)noexcept{
            HBuffer data;
            size_t wasAt = m_ReadPos;

            /// TODO: fix the null terminator
            if(sizeEncoded){
                uint32_t size = ReadUInt32(status, false);
                if(!status)return data;
                data.ReserveString(size);
                m_ReadPos+=sizeof(uint32_t);


                /// @brief we do size + 1 to include the null terminator that is suppose to be at the end.
                for(uint32_t i = 0; i < size + 1; i++){
                    int8_t byte = ReadInt8(status);
                    if(!status){
                        if(!moveReadPos)m_ReadPos = wasAt;
                        return data;
                    }
                    data.Append(byte);
                }
                if(!moveReadPos)m_ReadPos = wasAt;
                return data;
            }
            /// @brief Normal read. Start adding every byte until we reach a null terminator
            int8_t byte;
            do{
                byte = ReadInt8(status);
                if(!status)return data;
                data.Append(static_cast<char>(byte));
            }while(byte != '\0');

            if(!moveReadPos)m_ReadPos = wasAt;
            return data;
        }
    #pragma endregion
    
    public:
        HBuffer& GetBuffer()const noexcept{return (HBuffer&)m_Buffer;}
        uint32_t GetReadPos()const noexcept{return m_ReadPos;}
        uint32_t GetLength() const noexcept{return (std::min(static_cast<size_t>(4), static_cast<size_t>(m_ReadPos)) - 4);}
    private:
        HBuffer m_Buffer;
        /// @brief size of unsigned integer for packet size
        uint32_t m_ReadPos = 4;
    };
}