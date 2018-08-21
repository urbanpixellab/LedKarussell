//
//  LedAnimator.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#include "LedAnimator.hpp"

LedAnimator::LedAnimator(MidiControl * mc): _MC(mc)
{
    //only have to been called once;
    // is working on array getting in and function select + parameters like direction and color
    
    _aniSelect = 0; //old
    
    
    //load the selections from xml and create buttons for every selection with the name and id
    ofxXmlSettings sel("selector.xml");
    cout << "load xml" << endl;
    for (int i = 0; i < sel.getNumTags("select"); i++)
    {
        sel.pushTag("select",i);
        Selection s;
        s.name = sel.getValue("name", "untitled");
        cout << "segment name " << s.name << endl;
        //how many elements or selections?
        //maximum one layer down by now
        //still not working and empty
        
        /*
        for (int s = 0; s < sel.getNumTags("segment"); s++)
        {
            // get the already existing items and add this to the selection
            // check with _selections[]and fill up with them, must been recursive...
            // but later now only one level down
            // replace selection with already existin items in the selection based on the name
            sel.pushTag("selection",s);
            cout << sel.getValue("id", 0) << endl;
            sel.popTag();
//            s.items.push_back(sel.getValue("segment", 0));
        }*/
        //here the segments
        int segments = sel.getNumTags("segment");
        cout << segments << " segments" << endl;
        for (int seg = 0; seg < sel.getNumTags("segment"); seg++)
        {
            // get the already existing items and add this to the selection
            // replace selection with already existin items
            sel.pushTag("segment",seg);
            int id = sel.getValue("segment", 0);
            cout << id << endl;
            s.items.push_back(id);
            sel.popTag();
        }
        
        // now add the listet segments
        _selections.push_back(s);
        sel.popTag();
    }
    
}

LedAnimator::~LedAnimator()
{
}

//////////////////////niew

void LedAnimator::drawToArray(int drawFunction,u_int8_t * selectionArrays,int &length,ofVec3f colorFunctionSelect)
{
    //dt is already existing
    // plus later 3 offset and frequency parameter
    //example is simple sinewave on dT
    float freq = 3;
    switch (drawFunction) {
        case CURVE::BLACKOUT:
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < length; i++)
            {
                selectionArrays[i] = 0;
            }
            
            break;
            
        case CURVE::WHITEOUT:
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < length; i++)
            {
                selectionArrays[i] = 255;
            }
            
            break;

        case CURVE::RECT://add frequency in repetirion
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < (length/3); i++) //reduce to pixel
            {
                selectionArrays[(i * 3) + 0] = 0;
                selectionArrays[(i * 3) + 1] = 0;
                selectionArrays[(i * 3) + 2] = 0;
                if (i < _MC->getDt()* (length/3)) {
                    //two pi =
                    selectionArrays[(i * 3) + 0] = 255;
                    selectionArrays[(i * 3) + 1] = 255;
                    selectionArrays[(i * 3) + 2] = 255;
                }
            }
            
            break;

        case CURVE::SINE://blackout
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < (length/3); i++) //reduce to pixel
            {
                //two pi =
                selectionArrays[(i * 3) + 0] = 255 * (0.5+sin(i * 0.01*_MC->getDt()*TWO_PI)*2);
                selectionArrays[(i * 3) + 1] = 255 * (0.5+sin(i * 0.01*_MC->getDt()*TWO_PI)*2);
                selectionArrays[(i * 3) + 2] = 255 * (0.5+sin(i *0.01*_MC->getDt()*TWO_PI)*2);
            }
            
            break;
            
            
        default:
            break;
    }
}