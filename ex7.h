
// dvir yotvat 311212823

#include "wx/wx.h"
#include <queue>
#include<stdlib.h>
using namespace std;
#ifndef INCLUDED_HELLOWORLDAPP_H
#define INCLUDED_HELLOWORLDAPP_H

// the 8 directions:
const int R=1, L=2, U=3, D=4, UR=5, DR=6, UL=7, DL=8;

// ========================================================================================================
// myApp
// ========================================================================================================
class myApp : public wxApp
{
public:
    virtual bool OnInit();
};

// ========================================================================================================
// MyFrame
// ========================================================================================================
class MyCanvas;
class MyFrame : public wxFrame
{
public:
    MyFrame();

    MyCanvas *GetCanvas() const { return m_canvas; }
    void SetCanvas(MyCanvas *canvas) { m_canvas = canvas; }

private:
    MyCanvas *m_canvas;
};

// ========================================================================================================
// MyCanvas
// ========================================================================================================
class DragShape;
class MyCanvas : public wxScrolledWindow
{
public:
    MyCanvas(wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size);
    ~MyCanvas();

    void SetShape(DragShape *shape) { _shape = shape; }
    DragShape *GetShape() { return _shape; }

    void DrawShapes(wxDC &dc);
    void ClearShapes();

    // queue of shapes
    queue<DragShape *> myQ;

private:
    wxTimer m_timer;
    DragShape *_shape;

    wxDECLARE_EVENT_TABLE();

public:
    void OnPaint(wxPaintEvent &event);
    void OnTimer(wxTimerEvent &event);
    void OnChar(wxKeyEvent &event);
};

// ========================================================================================================
// DragShape
// ========================================================================================================
class DragShape : public wxObject
{
public:
    // the direction
    int direction;
    //counter of steps
    int counter;

    DragShape(const wxBitmap &bitmap);
    ~DragShape() {}

    //// Operations
    virtual bool Draw(wxDC &dc);

    //// Accessors
    virtual wxRect GetRect() const
    {
        return wxRect(m_pos.x, m_pos.y,
                      m_bitmap.GetWidth(), m_bitmap.GetHeight());
    }

    wxPoint GetPosition() const { return m_pos; }
    virtual void SetPosition(const wxPoint &pos) { m_pos = pos; }
    virtual void SetDirection(int direction1) { direction = direction1; }

    wxBitmap &GetBitmap() const { return (wxBitmap &)m_bitmap; }
    void SetBitmap(const wxBitmap &bitmap) { m_bitmap = bitmap; }

    // 8 const directions
    virtual void my_direction(){
    // right (R)
    if (this->direction==1)
        this->SetPosition(wxPoint(this->GetPosition().x+1, this->GetPosition().y));
    // left (L)
    else if (this->direction==2) 
        this->SetPosition(wxPoint(this->GetPosition().x-1, this->GetPosition().y));
    // up (U)
    else if (this->direction==3) 
        this->SetPosition(wxPoint(this->GetPosition().x, this->GetPosition().y-1));
    // down (D)
    else if (this->direction==4) 
        this->SetPosition(wxPoint(this->GetPosition().x, this->GetPosition().y+1));
    // up and right (UR)
    else if (this->direction==5) 
        this->SetPosition(wxPoint(this->GetPosition().x+1, this->GetPosition().y-1));
    // down and right (DR)
    else if (this->direction==6) 
        this->SetPosition(wxPoint(this->GetPosition().x+1, this->GetPosition().y+1));
    // up and left (UL)
    else if (this->direction==7) 
        this->SetPosition(wxPoint(this->GetPosition().x-1, this->GetPosition().y-1));
    // down and left (DL)
    else
        this->SetPosition(wxPoint(this->GetPosition().x-1, this->GetPosition().y+1));
    }
protected:
    wxPoint m_pos;
    wxBitmap m_bitmap;
};


// ========================================================================================================
// PairShape
// ========================================================================================================
class PairShape : public DragShape
{
public:
    // pointers of shapes
    DragShape *pointer1, *pointer2;

    int direction;
    //counter of steps
    int count;

    // set a new direction
    virtual void SetDirection(int new_direction){
        direction=new_direction;
        pointer1->SetDirection(new_direction);
        pointer2->SetDirection(new_direction);
    }

    // the direction of the 2 shapes
    virtual void my_direction(){
        pointer1->my_direction();
        pointer2->my_direction();
    }

    // get the position of the 2 shapes
    virtual wxPoint GetPosition() const{
        return pointer1->GetPosition();
    }

    // draw the shape 1 and shape 2 (the pionters)
    virtual bool Draw(wxDC &dc){

        if (m_bitmap.IsOk()){

            wxMemoryDC memDC;
            memDC.SelectObject(pointer1->GetBitmap());
            dc.Blit(pointer1->GetPosition().x, pointer1->GetPosition().y, pointer1->GetBitmap().GetWidth(), pointer1->GetBitmap().GetHeight(),&memDC, 0, 0, wxCOPY, true);

            wxMemoryDC memDC2;
            memDC2.SelectObject(pointer2->GetBitmap());
            dc.Blit(pointer2->GetPosition().x, pointer2->GetPosition().y, pointer2->GetBitmap().GetWidth(), pointer2->GetBitmap().GetHeight(),&memDC2, 0, 0, wxCOPY, true);

            return true;
    }
        else
            return false;
    }

    // get the rec of the 2 shapes 
    virtual wxRect GetRect() const{
        return (pointer1->GetRect()).Union(pointer2->GetRect());
    }
    
    // pair the 2 shapes together 
    PairShape(DragShape *p1, DragShape *p2) : DragShape(p1->GetBitmap()){

        pointer1=p1;
        pointer2=p2;
        this->direction=p1->direction;
        p2->direction=p1->direction;
    }

};

#endif
