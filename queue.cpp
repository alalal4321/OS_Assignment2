#include <iostream>
#include "queue.h"
#include <cstdlib>  // malloc, free

using namespace std;


Queue* init()
{
    auto* q = new (std::nothrow) Queue;   
    if (!q) return nullptr;

    q->head = q->tail = nullptr;
    return q;
}


void release(Queue* q)
{
    if (!q) return;
    Node* cur = q->head;
    while (cur) { Node* nxt = cur->next; nfree(cur); cur = nxt; }
    delete q;                              
}


Node* nalloc(Item it)
{
    Node* n = static_cast<Node*>(std::malloc(sizeof(Node)));
    if (!n) {
        std::cerr << "[] nalloc malloc 실패\n";
        return nullptr;
    }
    n->item = it;
    n->next = nullptr;
    return n;
}



void nfree(Node* n)
{
    if (n) std::free(n);
}


Node* nclone(Node* n)            
{
    if (!n) return nullptr;
    Node* cp = nalloc(n->item);
    return cp;
}



Reply enqueue(Queue* q, Item it)
{
    Reply rep{ false, it };
    if (!q) return rep;

    std::lock_guard<std::mutex> g(q->lock);   // 단일 락

    Node* n = nalloc(it);
    if (!n) return rep;

    // 빈 큐
    if (!q->head) {
        q->head = q->tail = n;
        rep.success = true;
        return rep;
    }

    // head 앞(내림차순이므로 key 가 더 클 때) 
    if (it.key > q->head->item.key) {
        n->next = q->head;
        q->head = n;
        rep.success = true;
        return rep;
    }

    // 중간 / 끝 
    Node* cur = q->head;
    int   cnt = 0;
    const int MAX_LOOP = 100000;      // 무한 루프 안전장치

    while (cur->next && cur->next->item.key > it.key) {
        if (++cnt > MAX_LOOP) {
            std::cerr << "[ERR] enqueue loop overflow  key=" << it.key << '\n';
            break;                        
        }
        cur = cur->next;
    }

    // 삽입 
    n->next = cur->next;      // 사본
    cur->next = n;              // 연결
    if (!n->next) q->tail = n;  // tail 갱신

    rep.success = true;
    return rep;
}




Reply dequeue(Queue* q)
{
    Reply rep{ false, {} };
    if (!q) return rep;

    std::lock_guard<std::mutex> g(q->lock);

    if (!q->head) return rep;        // 비어있을때

    Node* tgt = q->head;
    rep.item = tgt->item;
    rep.success = true;

    q->head = tgt->next;
    if (!q->head) q->tail = nullptr;

    nfree(tgt);
    return rep;
}


Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
