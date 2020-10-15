
// dvir yotvat 311212823

#include "wx/wx.h"
#include <wx/timer.h>
#include <queue>
#include "ex7.h"
#include <cstdio>
#include<stdlib.h>
#include<time.h>
using namespace std;

IMPLEMENT_APP(myApp)

// ========================================================================================================
// wxApp
// ========================================================================================================
bool myApp::OnInit()
{
    MyFrame *frame = new MyFrame(); 
	frame->Show(true);

 	wxImage image;
    wxImage::AddHandler( new wxPNGHandler );
    
    // the user shape
    bool isloadOK = image.LoadFile("images1.png", wxBITMAP_TYPE_PNG);
    if (isloadOK){
        DragShape *newShape = new DragShape(wxBitmap(image));
        newShape->SetPosition(wxPoint(50, 50));
        frame->GetCanvas()->SetShape(newShape);
    }
    // the pc shapes

    wxImage image2;
    wxImage::AddHandler(new wxPNGHandler);
    bool isloadOK2=image2.LoadFile("images.png", wxBITMAP_TYPE_PNG);
    if (isloadOK2){
        DragShape *newShape2=new DragShape(wxBitmap(image2));
        newShape2->SetPosition(wxPoint(360, 220));
        frame->GetCanvas()->myQ.push(newShape2);
    }
    wxImage image3;
    wxImage::AddHandler(new wxPNGHandler);
    bool isloadOK3=image3.LoadFile("images.png", wxBITMAP_TYPE_PNG);
    if (isloadOK3){
        DragShape *newShape3=new DragShape(wxBitmap(image3));
        newShape3->SetPosition(wxPoint(420, 40));
        frame->GetCanvas()->myQ.push(newShape3);
    }
    wxImage image4;
    wxImage::AddHandler(new wxPNGHandler);
    bool isloadOK4=image4.LoadFile("images.png", wxBITMAP_TYPE_PNG);
    if (isloadOK4){
        DragShape *newShape4=new DragShape(wxBitmap(image4));
        newShape4->SetPosition(wxPoint(80, 250));
        frame->GetCanvas()->myQ.push(newShape4);
    }

    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

// ========================================================================================================
// MyFrame
// ========================================================================================================
MyFrame::MyFrame()
    : wxFrame((wxFrame *)NULL, wxID_ANY, "EX 7",
              wxPoint(20, 20), wxSize(470, 360))
{
    wxSize sz = GetClientSize();
    m_canvas = new MyCanvas(this, wxID_ANY, wxPoint(0, 0), wxSize(sz.GetX(), sz.GetY()));
}

// ========================================================================================================
// MyCanvas
// ========================================================================================================
wxBEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
        EVT_TIMER(wxID_FILE9, MyCanvas::OnTimer)
            wxEND_EVENT_TABLE()

    // ----------------------------------------------------------------------------
    MyCanvas::MyCanvas(wxWindow *parent, wxWindowID id,
                       const wxPoint &pos, const wxSize &size)
    : wxScrolledWindow(parent, id, pos, size, wxSUNKEN_BORDER), m_timer(this, wxID_FILE9)
{
    m_timer.Start(50);
    Bind(wxEVT_CHAR_HOOK, &MyCanvas::OnChar, this);
    SetBackgroundColour(*wxWHITE);
    SetCursor(wxCursor(wxCURSOR_ARROW));
}

// ----------------------------------------------------------------------------
void MyCanvas::OnTimer(wxTimerEvent &event)
{

    // 2 pointers 
    DragShape *p1;
    DragShape *p2;
    // temps queue that help to save the orgin queue
    queue<DragShape *> temp;

	// the user shape move only with keyboard
    _shape->SetPosition(wxPoint(_shape->GetPosition().x,_shape->GetPosition().y));
    Refresh(true);
    Update(); 

    p1=myQ.front();
    myQ.pop();

    // check if you won or lose
    while (!myQ.empty()){

        p2=myQ.front();
        myQ.pop();

        // if i impact shape 1 i lose 
        if((_shape->GetRect()).Intersects(p1->GetRect())){ 
            wxMessageBox(wxT("Game Over - you LOSE :-("));
        exit(1);
        }
        // if i impact shape 2 i lose
        if ((_shape->GetRect()).Intersects(p2->GetRect())){ 
            wxMessageBox(wxT("Game Over - you LOSE :-("));
            exit(1);
        }
        // if shape 1 impact  shape 2
        if ((p1->GetRect()).Intersects(p2->GetRect())){
            // if shape 1 impact  shape 2 and the 2 queue empty so no more shapes left and i won
            if((myQ.empty())&&(temp.empty())){ 
                wxMessageBox(wxT("Game Over - you WIN :-)"));
                exit(1);
            }
            // if shape 1 impact  shape 2 need to pair theme
            p1=new PairShape(p1, p2);
        }
        else
            temp.push(p2);
    }
    temp.push(p1);
    myQ=temp;
    Refresh(true);
    Update();
}

// ----------------------------------------------------------------------------
void MyCanvas::OnChar(wxKeyEvent &event)
{

    int x = 0, y = 0;
    if (event.GetUnicodeKey() != WXK_NONE)
        return;
    // It's a special key, deal with all the known ones:
    switch (event.GetKeyCode())
    {
    case WXK_NUMPAD_LEFT:
    case WXK_LEFT:
        x--;
        break;
    case WXK_NUMPAD_RIGHT:
    case WXK_RIGHT:
        x++;
        break;
    case WXK_NUMPAD_UP:
    case WXK_UP:
        y--;
        break;
    case WXK_NUMPAD_DOWN:
    case WXK_DOWN:
        y++;
        break;
    }
    _shape->SetPosition(wxPoint(_shape->GetPosition().x + x, _shape->GetPosition().y + y));

    Refresh(true);
    Update();
}

// ----------------------------------------------------------------------------
void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);
    DrawShapes(dc);
}

// ----------------------------------------------------------------------------
void MyCanvas::DrawShapes(wxDC &dc)
{
    // pointer
    DragShape *p1;
    // temp queue (help to save the original queue)
    queue<DragShape *> temp;
    
    while(!myQ.empty()){ 
        p1=myQ.front();
        myQ.pop();
        p1->counter++;
        // if the shape get to 170 steps
        if(p1->counter==170){
            // chose a num between 1-8 and set a new direction
            p1->SetDirection(rand()%8+1);
            p1->counter=0;
        }
        // move the shape to the direction
        p1->my_direction();

        // the locations of the "DragShape"
        wxRect r = p1->GetRect();
        int posX = r.GetX();
        int posY = r.GetY();
        int height = r.GetHeight();
        int width = r.GetWidth();

        // all the options that the shape can impact the wall
        if(posX<=0){
            switch (p1->direction){
                case L:
                    p1->SetDirection(R);
                    break;
                case UL:
                    p1->SetDirection(UR);
                    break;
                default:
                    p1->SetDirection(DR);
                    break;
            }
        }
        if(posX+width>=470){
            switch (p1->direction){
                case R:
                    p1->SetDirection(L);
                    break;
                case UR:
                    p1->SetDirection(UL);
                    break;
                default:
                    p1->SetDirection(DL);
                    break;
            }
        }
        if(posY<=0){
            switch (p1->direction){
                case U:
                    p1->SetDirection(D);
                    break;
                case UR:
                    p1->SetDirection(DL);
                    break;
                default:
                    p1->SetDirection(DR);
                    break;
            }
        }
        if(posY+height>=340){
            switch (p1->direction){
                case D:
                    p1->SetDirection(U);
                    break;
                case DL:
                    p1->SetDirection(UL);
                    break;
                default:
                    p1->SetDirection(UR);
                    break;
            }
        }
        temp.push(p1);
        // draw the shape of the user
        _shape->Draw(dc);
        // draw the shape of the pc
        p1->Draw(dc);
    }
    myQ=temp;    
}

// ----------------------------------------------------------------------------
MyCanvas::~MyCanvas()
{
}

// ========================================================================================================
// DragShape
// ========================================================================================================
DragShape::DragShape(const wxBitmap &bitmap)
{
    counter = 0;
    direction = (rand() % 8 )+ 1;
    m_bitmap = bitmap;
    m_pos.x = 0;
    m_pos.y = 0;
}

// ----------------------------------------------------------------------------
bool DragShape::Draw(wxDC &dc)
{
    if (m_bitmap.IsOk())
    {
        wxMemoryDC memDC;
        memDC.SelectObject(m_bitmap);

        dc.Blit(m_pos.x, m_pos.y, m_bitmap.GetWidth(), m_bitmap.GetHeight(),
                &memDC, 0, 0, wxCOPY, true);

        return true;
    }
    else
        return false;
}

