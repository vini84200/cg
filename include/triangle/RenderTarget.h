//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERTARGET_H
#define TRIANGLE_RENDERTARGET_H


class RenderTarget {

public:
    virtual void activate() = 0;
    virtual void deactivate() = 0;

};


#endif //TRIANGLE_RENDERTARGET_H
