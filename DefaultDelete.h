//
// Created by slonik on 1/16/22.
//

#ifndef SMARTPOINTERS_DEFAULTDELETE_H
#define SMARTPOINTERS_DEFAULTDELETE_H

template <typename T>
class DefaultDelete {
public:
    void operator()(T* pT){
        delete pT;
    }
};

#endif //SMARTPOINTERS_DEFAULTDELETE_H
