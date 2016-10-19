// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct CollectionNode
{
    struct CollectionNode* m_Prev;
    struct CollectionNode* m_Next;
    char m_Data[0];
};

struct Queue
{
    size_t m_TypeSize;
    int m_Count;
    struct CollectionNode* m_Head;
    struct CollectionNode* m_Tail;

    void InitInPlace(size_t type_size)
    {
        m_TypeSize = type_size;
        m_Count = 0;
        m_Head = NULL;
        m_Tail = NULL;
    }
    
    void Destroy()
    {
        char temp[m_TypeSize];
        while (Count())
            Dequeue(temp);
    }
    
    void Enqueue(void* data)
    {
        CollectionNode* node = (CollectionNode* ) malloc(sizeof *node + m_TypeSize);
        memcpy(&node->m_Data[0], data, m_TypeSize);
        m_Count++;
        
        if (!m_Head)
        {
            // first entry
            m_Head = m_Tail = node;
            node->m_Next = NULL;
            node->m_Prev = NULL;
            return;
        }
        
        m_Tail->m_Next = node;
        node->m_Prev = m_Tail;
        node->m_Next = NULL;
        
        // node is our new tail
        m_Tail = node;
    }
    
    void Dequeue(void* data)
    {
        CollectionNode* node = m_Head;
    
        if (m_Count == 0)
            return;
    
        if (m_Head->m_Next)
            m_Head->m_Next->m_Prev = NULL;
    
        if (m_Tail == m_Head)
            m_Head = m_Tail = NULL;
        else
            m_Head = m_Head->m_Next;
    
        memcpy(data, &node->m_Data[0], m_TypeSize);
        free(node);
        m_Count--;
    }

    inline int Count() const
    {
        return m_Count;
    }
};
