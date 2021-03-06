//
//  Scud.h
//
//  Created by Boris Vigman on 17/05/2017.
//  Copyright © 2017 Boris Vigman. All rights reserved.
//
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.
 
 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef Scud_h
#define Scud_h

#define SCUD_VERSION "0.1.13"

//#define SCUD_USE_EXCEPTIONS 1
//#define SCUD_DEBUG_MODE_ENABLED 1
//#define SCUD_CUSTOM_MUTEX_AVAILABLE 1
//#define SCUD_CUSTOM_RNG_AVAILABLE 1
//#define SCUD_CUSTOM_QUEUE_AVAILABLE 1
//#define SCUD_CUSTOM_MAP_AVAILABLE 1
//#define SCUD_CUSTOM_VECTOR_AVAILABLE 1

//------------- EXPERIMENTAL! PLEASE DO NOT UNCOMMENT NEXT LINES!
//#define SCUD_WFQ_AVAILABLE 1
//#define SCUD_CUSTOM_MINORDERED_LIST_AVAILABLE 1
//---------------------------------------------------------------
#define SCUD_IOSTREAM_AVAILABLE
#define SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES 64
#define SCUD_DRR_QUANTUM 10
#define SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT 8
#define SCUD_QUEUE_DEFAULT_LOW_THRESHOLD 0
#define SCUD_QUEUE_DEFAULT_HIGH_THRESHOLD 100

#ifdef SCUD_WFQ_AVAILABLE
#define SCUD_WFQ_LOAD_AT_ONCE_LINKS_NUM 100
#define SCUD_WFQ_MIN_POSSIBLE_WEIGHT 0.0000000000001
#define SCUD_WFQ_LINK_POLLING_BATCH_SIZE 100
#endif

//-------------------------------------------------------
//         PLEASE DO NOT EDIT BELOW THIS LINE
//-------------------------------------------------------
#ifdef SCUD_IOSTREAM_AVAILABLE
#include <iostream>
#endif
#ifndef SCUD_CUSTOM_MUTEX_AVAILABLE
#include "mutex"
#endif
#ifndef SCUD_CUSTOM_MAP_AVAILABLE
#include "unordered_map"
#endif
#ifndef SCUD_CUSTOM_QUEUE_AVAILABLE
#include "deque"
#endif
#ifndef SCUD_CUSTOM_VECTOR_AVAILABLE
#include "vector"
#endif
#ifndef SCUD_CUSTOM_MINORDERED_LIST_AVAILABLE
#include "queue"
#endif

#ifdef SCUD_USE_EXCEPTIONS
#define SCUD_THROW_EXCEPTION(x) throw (x)
#else
#define SCUD_THROW_EXCEPTION(x)
#endif

#ifdef SCUD_DEBUG_MODE_ENABLED
#include <string>

#define SCUD_PRINT_STR(x) {std::string s=(x);std::cout<<s<<std::endl;}
#define SCUD_PRINT_STR_WITH_PARAM(x,param) {std::string s=(x);std::cout<<s<<(param)<<std::endl;}
#else
#define SCUD_PRINT_STR(x)
#define SCUD_PRINT_STR_WITH_PARAM(x,param)
#endif

#define SCUD_MAX_POSSIBLE_PRIORITY 127
#if SCUD_MAX_POSSIBLE_PRIORITY+1 < SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES
#error "Maximum available priority is greater than maximum allowed priority"
#endif

namespace SCUD{
    typedef enum {
        SCUD_RC_OK=0,
        SCUD_RC_FAIL_INVALID_PARAM,
        SCUD_RC_FAIL_INVALID_PRIORITY,
        SCUD_RC_FAIL_INVALID_WEIGHT,
        SCUD_RC_FAIL_INVALID_DRR_QUANTUM,
        SCUD_RC_FAIL_INVALID_SCHEDULING_PARAM,
        SCUD_RC_FAIL_LINK_EXISTS,
        SCUD_RC_FAIL_LINK_NOT_EXISTS,
        SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED,
        SCUD_RC_FAIL_ILLEGAL_OP,
        SCUD_RC_LINK_FAILED,
        SCUD_RC_FAIL_LINK_HAS_PREDECESSOR,
        SCUD_RC_FAIL_LINK_HAS_SUCCESSOR,
        SCUD_RC_FAIL_LINK_ID_NOT_SET,
        SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD,
        SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD,
        SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE,
        SCUD_RC_FAIL_OBJ_PROPAGATION_FAILED
    } SCUD_RC;
#ifdef SCUD_WFQ_AVAILABLE
    typedef long long SCUDTimestamp;
#endif
    typedef struct _Prim{
        SCUD_RC retCode;
        
        union {
            long long signedInt;
            long Long;
            short Short;
            long double real;
            unsigned long long unsignedInt;
            unsigned long unsignedLong;
            unsigned short unsignedShort;
            
            struct{
                char* ptr;
                long long size;
            } ptr;
            
        } primitives;
        _Prim(){
            retCode=SCUD_RC_OK;
        }
    } SchedulablePayload;

#ifndef SCUD_CUSTOM_VECTOR_AVAILABLE
    template<typename T> class SCVector{
    public:
        std::vector<T> itsvector;
        void push_back(T& el){
            itsvector.push_back(el);
        }
        T elementAt(long i){
            if(i<0||i>=itsvector.size()){
                T t;
                return t;
            }
            return itsvector.at(i);
        }
        long size(){
            return itsvector.size();
        }
        void clear(){
            itsvector.clear();
        }
    };
#else
#include "scud_custom_vector.h"
#endif
#ifndef SCUD_CUSTOM_MAP_AVAILABLE
template<typename Tid, typename Container> class SCMap{
        std::unordered_map<Tid,Container> itsmap;
        typename std::unordered_map<Tid,Container>::iterator itsit;
    public:
        SCMap(){
            resetIterator();
        }
        long long size(){return itsmap.size();};
        
        Container getCurrentContent(){
            if(itsit==itsmap.end()){
                itsit=itsmap.begin();
            }
            return itsit->second;
        }
        Tid getCurrentId(){
            return itsit->first;
        }
        void setContent(Tid& t,Container& c){
            itsmap[t]=c;
        }
        void resetIterator(){
            itsit=itsmap.begin();
        };
        void insert(Tid& tid,Container& c){
            itsmap.insert(std::make_pair(tid, c));
        };
        void clear(){
            itsmap.clear();itsit=itsmap.begin();
        }
        void erase(Tid& t){
            itsmap.erase(t);
        };
        Container find(Tid& t,bool& res){
            res=true;
            typename std::unordered_map<Tid,Container>::iterator icit=itsmap.find(t);
            if(icit==itsmap.end()){
                Container c;
                res=false;
                return c;
            }
            return icit->second;
        };
        bool isExgausted(){
            if(itsit==itsmap.end())
                return true;
            return false;
        }
        bool exists(Tid& id){
            if(itsmap.find(id)==itsmap.end())
                return false;
            return true;
        }
        void promoteIterator(){
            if(itsit==itsmap.end()){
                itsit=itsmap.begin();
            }
            else{
                ++itsit;
            }
        }
    void promoteIteratorSafely(){
        if(itsit==itsmap.end()){
            itsit=itsmap.begin();
        }
        else{
            ++itsit;
        }
        if(itsit==itsmap.end()){
            itsit=itsmap.begin();
        }
    }
    };
#else
#include "scud_custom_map.h"
#endif

#ifndef SCUD_CUSTOM_QUEUE_AVAILABLE
template<typename T> class SCQueue{
        std::deque<T> deq;
    public:
        void push_front(T& sch){
            deq.push_front(sch);
        }
        void pop_back(){
            if(deq.size()>0){
                deq.pop_back();
            }
        }
        void back(T& sch){
            if(deq.size()>0){
                sch=deq.back();
            }
        }
        void clear(){deq.clear();}
        long long size(){return deq.size();}
    };
#else
#include "scud_custom_queue.h"
#endif
#ifndef SCUD_CUSTOM_MINORDERED_LIST_AVAILABLE
    template<typename Tid,typename T1, typename T2, typename T3> class SCMinOrderedList{
        std::priority_queue<T1,T2,T3> mypq;
        std::unordered_map<Tid, char> uniqueItems;
    public:
        void push(Tid itsId,T1& element){
            typename std::unordered_map<Tid,char>::const_iterator it = uniqueItems.find (itsId);
            if(it==uniqueItems.end()){
                mypq.push(element);
                uniqueItems[itsId]=0;
            }
        }
        T1 top(){
            return mypq.top();
        }
        void pop(Tid itsId){
            mypq.pop();
            uniqueItems.erase(itsId);
        }
        long long size(){
            return mypq.size();
        }
        void clear(){
            mypq.clear();
            uniqueItems.clear();
        }
        
    };
#else
    #include "scud_custom_minordered_list.h"
#endif
  
#ifdef SCUD_WFQ_AVAILABLE
class SCTime{
public:
    SCTime();
    //long long getCurrentTime();
    static SCUDTimestamp getCurrentTime();
    ~SCTime();
};
#endif
    
#ifndef SCUD_CUSTOM_RNG_AVAILABLE
#include <stdlib.h>
#include <time.h>
class SCRng{
    public:
        SCRng(){
            srand ((unsigned int)time(NULL));
        };
        //returns random number in range 0..RAND_MAX
        int randomInt(){
            
            return rand();
        };
        //returns random number in range (0..1)
        float randomFloat(){
            return rand()/((float)RAND_MAX);
        };
        virtual ~SCRng(){};
    };
#else
class SCRng{
    public:
        SCRng();
        //returns random number in range 0..RAND_MAX
        int randomInt();
        //returns random number in range (0..1)
        float randomFloat();
        virtual ~SCRng();
    };
#endif

#ifndef SCUD_CUSTOM_MUTEX_AVAILABLE
    class SCLocker{
        std::mutex mut;
    public:
        SCLocker(){};
        void lock(){
            mut.lock();
        };
        void unlock(){
            mut.unlock();
        };
        virtual ~SCLocker(){};
    };
#else
    class SCLocker{
    public:
        SCLocker();
        void lock();
        void unlock();
        virtual ~SCLocker();
    };
#endif

class SCHelper{
    static char itsVersion[];
    static long long llcount;
public:
    static long long newCount(){return ++llcount;};
    static char* version(){
         return itsVersion;
     };
#ifdef SCUD_IOSTREAM_AVAILABLE
     static std::string convertReturnCodeToString(SCUD_RC val){
            std::string result="Result: undefined";
            switch (val) {
                case SCUD_RC_OK:result="Result: OK";break;
                case SCUD_RC_FAIL_INVALID_PARAM:result="Result: FAIL: invalid param provided";break;
                case SCUD_RC_FAIL_INVALID_PRIORITY:result="FAIL: invalid priority";break;
                case SCUD_RC_FAIL_INVALID_WEIGHT:result="FAIL: invalid weight";break;
                case SCUD_RC_FAIL_LINK_EXISTS:result="FAIL: link already exists";break;
                case SCUD_RC_FAIL_LINK_NOT_EXISTS:result="FAIL: link not exists";break;
                case SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED:result="Result: Self referencing link";break;
                case SCUD_RC_FAIL_ILLEGAL_OP:result="FAIL: illegal operation requested";break;
                case SCUD_RC_LINK_FAILED:result="FAIL: link operation failed";break;
                case SCUD_RC_FAIL_LINK_HAS_PREDECESSOR:result="FAIL: predecessor exists";break;
                case SCUD_RC_FAIL_LINK_HAS_SUCCESSOR:result="FAIL: successor exists";break;
                case SCUD_RC_FAIL_LINK_ID_NOT_SET:result="FAIL: link ID not set";break;
                case SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD:result="FAIL: under low threshold";break;
                case SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD:result="FAIL: above high threshold";break;
                case SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE:result="FAIL: no object is available";break;
                case SCUD_RC_FAIL_OBJ_PROPAGATION_FAILED:result="FAIL: object propagation failed";break;
                case SCUD_RC_FAIL_INVALID_DRR_QUANTUM:result="Invalid DRR quantum, must be strictly positive";break;
                case SCUD_RC_FAIL_INVALID_SCHEDULING_PARAM:result="Invalid scheduling parameter, must be strictly positive";break;
                default:
                    break;
            }
            return result;
        }
#endif
    };
    
    template<typename TSchedulable,typename Tid> class LinkableDropper ;
    template<typename TSchedulable,typename Tid> class LinkableQueue;
    template<typename TSchedulable,typename Tid> class LinkableScheduler ;
    template<typename TSchedulable,typename Tid> class LinkableSchedulerPriority;
    template<typename TSchedulable,typename Tid> class LinkableSchedulerNaiveRR;
    template<typename TSchedulable,typename Tid> class LinkableSchedulerDRR;
#ifdef SCUD_WFQ_AVAILABLE
    template<typename TSchedulable,typename Tid> class LinkableSchedulerWFQ;
#endif
    template<typename TSchedulable,typename Tid> class LinkableNull;
    template<typename TSchedulable,typename Tid> class LinkablePass;
    /*
     -----------------------------------------------------------------------------
     LINKABLE
     
     -----------------------------------------------------------------------------
     
     */
    template<typename TSchedulable, typename Tid> class Linkable{
    public:
        struct Queueable{
            TSchedulable scheduled;
            long long schParam;
#ifdef SCUD_WFQ_AVAILABLE
            SCUDTimestamp timestamp;
            float weight;
            float prevWeight;
#endif
            Queueable(){schParam=-1;};
        };
    protected:
#ifdef SCUD_DEBUG_MODE_ENABLED
        std::string elementClass;
#endif
        Tid itsId;
        SCLocker lockerLinkable;
        
    protected:
        struct SchedulingProperties{
#ifdef SCUD_WFQ_AVAILABLE
            float weight;
            float prevWeight;
#endif
            char priority;
            SchedulingProperties(){
#ifdef SCUD_WFQ_AVAILABLE
                weight=-1;
#endif
                priority=-1;
            }
            SchedulingProperties(float w,char p){
#ifdef SCUD_WFQ_AVAILABLE
                weight=w;
#endif
                priority=p;
            }
        };
        typedef struct _sDRRControlInfo{
            bool ignoreDrr;
        }sDRRControlInfo;
        typedef union _uSchedulerControlInfo{
            sDRRControlInfo ssciDRR;
        }uSchedulerControlInfo;
        
        friend class LinkableQueue<TSchedulable,Tid>;
        friend class LinkableDropper<TSchedulable,Tid>;
        friend class LinkableScheduler<TSchedulable,Tid>;
        friend class LinkableSchedulerPriority<TSchedulable,Tid>;
        friend class LinkableNull<TSchedulable,Tid>;
        friend class LinkablePass<TSchedulable,Tid>;
#ifdef SCUD_WFQ_AVAILABLE
        friend class LinkableSchedulerWFQ<TSchedulable,Tid>;
#endif
        Linkable<TSchedulable,Tid>* next;
        Linkable<TSchedulable,Tid>* prev;
        SchedulingProperties scp;
        Tid getId(){
            return itsId;
        };
        void setId(Tid tid){
            itsId=tid;
        };
        virtual void _signalAvailability(bool canPull, long long countAvailable, float weight,char priority)=0;
        virtual SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu, typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI)=0;
        virtual SCUD_RC _unlinkPredecessor(Linkable<TSchedulable,Tid>*  link){
            SCUD_PRINT_STR("enter Linkable::_unlinkPredecessor");
            lockerLinkable.lock();
            prev=0;
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::_unlinkPredecessor");
            return SCUD_RC_OK;
        };
        virtual SCUD_RC _unlinkSuccessor(Linkable<TSchedulable,Tid>*  link){
            SCUD_PRINT_STR("enter Linkable::_unlinkSuccessor");
            lockerLinkable.lock();
            next=0;
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::_unlinkSuccessor");
            return SCUD_RC_OK;
        };
        virtual SCUD_RC _linkPredecessor(Linkable<TSchedulable,Tid>*  link){
            SCUD_PRINT_STR("enter Linkable::_linkPredecessor");
            lockerLinkable.lock();
            prev=link;
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::_linkPredecessor");
            return SCUD_RC_OK;
        };
        virtual SCUD_RC _linkSuccessor(Linkable<TSchedulable,Tid>*  link){
            SCUD_PRINT_STR("enter Linkable::_linkSuccessor");
            lockerLinkable.lock();
            next=link;
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::_linkSuccessor");
            return SCUD_RC_OK;
        };
        virtual SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>*  link,SchedulingProperties scps){
            return SCUD_RC_OK;
        };
        virtual SCUD_RC _simulatePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            struct Linkable<TSchedulable,Tid>::Queueable temp;
            this->lockerLinkable.lock();
            temp.schParam=-1;
#ifdef SCUD_WFQ_AVAILABLE
            temp.weight=this->scp.weight;
            if(this->scp.weight<SCUD_WFQ_MIN_POSSIBLE_WEIGHT){
                SCUD_PRINT_STR("WARNING: weight is less than possible minimum");
            }
#endif
            this->lockerLinkable.unlock();
            SCUD_RC rc=SCUD_RC_OK;
            qu=temp;
            return rc;
        };
    protected:
        Linkable():next(0),prev(0){
#ifdef SCUD_WFQ_AVAILABLE
            scp.weight=SCUD_WFQ_MIN_POSSIBLE_WEIGHT;
            scp.prevWeight=SCUD_WFQ_MIN_POSSIBLE_WEIGHT;
#endif
            scp.priority=-1;
#ifdef SCUD_DEBUG_MODE_ENABLED
            elementClass="Linkable";
#endif
        };

        virtual ~Linkable(){
            SCUD_PRINT_STR("enter Linkable::~Linkable");
            lockerLinkable.lock();
            prev=0;
            next=0;
            lockerLinkable.unlock();
            
            SCUD_PRINT_STR("exit Linkable::~Linkable");
        };

    public:
        struct LinkedObjectsTuple {
            SCVector<Linkable<TSchedulable,Tid>* > prevObject;
            Linkable<TSchedulable,Tid>* thisObject;
            Linkable<TSchedulable,Tid>* nextObject;
            LinkedObjectsTuple(Linkable<TSchedulable,Tid>* p,Linkable<TSchedulable,Tid>* t,Linkable<TSchedulable,Tid>* n){
                if(p){
                    prevObject.push_back(p);
                }
                thisObject=t;
                nextObject=n;
            };
        } ;
        virtual void processOnPullPush(TSchedulable sch, long long schedulingParam){
            
        };
        virtual void processOnPull(TSchedulable sch, long long schedulingParam){
            
        };
        virtual void processOnPush(TSchedulable sch, long long schedulingParam){
            
        };
        //pull object from previous link if such exists
        virtual SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu)=0;
        //Push object along with scheduling property
        virtual SCUD_RC push(TSchedulable sch, long long schedulingParam)=0;
        //Pull object if available from previous link if such exists and push to next link if exists
        virtual SCUD_RC pullAndPush(){
            SCUD_RC retcode=SCUD_RC_OK;
            SCUD_PRINT_STR("enter Linkable::pullAndPush");
            struct Linkable<TSchedulable,Tid>::Queueable ts;
            retcode=this->pull(ts);
            lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            lockerLinkable.unlock();
            if(retcode==SCUD_RC_OK && n){
                this->processOnPullPush(ts.scheduled,ts.schParam);
                n->push(ts.scheduled,ts.schParam);
                
            }else{
                SCUD_PRINT_STR("FAIL Linkable::pullAndPush");
                //retcode=SCUD_RC_FAIL_OBJ_PROPAGATION_FAILED;
            }
            
            SCUD_PRINT_STR("exit Linkable::pullAndPush");
            return retcode;
        };
        
        virtual bool canPull()=0;
        virtual bool hasBefore(){
            SCUD_PRINT_STR("enter Linkable::hasBefore");
            bool res=false;
            lockerLinkable.lock();
            if(this->prev){
                res=true;
            }
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::hasBefore");
            return res;
        }
        virtual bool hasAfter(){
            SCUD_PRINT_STR("enter Linkable::hasAfter");
            bool res=false;
            lockerLinkable.lock();
            if(this->next){
                res=true;
            }
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::hasAfter");
            return res;
        }
        
        virtual SCUD_RC setPriority( char prio){
            if(prio<0 || prio>=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES){
                SCUD_PRINT_STR("Linkable::setPriority - Attepmt of setting invalid priority:");
                SCUD_PRINT_STR("new priority is bigger than maximum possible or is less than 0");
                SCUD_PRINT_STR_WITH_PARAM("maximum possible priority is ", SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES-1);
                return SCUD_RC_FAIL_INVALID_PRIORITY;
            }
            
            lockerLinkable.lock();
            this->scp.priority=prio;
            if(next){

                next->_propagateSchedulingProperties(this,this->scp);
            }
            lockerLinkable.unlock();
            return SCUD_RC_OK;
        };
        
        char getPriority(){
            lockerLinkable.lock();
            char priority=this->scp.priority;
            lockerLinkable.unlock();
            return priority;
        };
        
        //Remove given element from chain. Rules of removal for
        //Scheduler: removes given scheduler itself and all prepended elements
        //Dropper: removes only given Dropper itself
        //Queue: removes only given Queue
        virtual LinkedObjectsTuple unlink(SCUD_RC* rc) {
            SCUD_PRINT_STR("enter Linkable::unlink");
            LinkedObjectsTuple ptn(0,this,0) ;
            lockerLinkable.lock();
            if(next!=0){
                SCUD_PRINT_STR("Linkable::unlink - next not empty");
                ptn.nextObject=next;
                next->_unlinkPredecessor(this);
                next=0;
            }
            if(prev!=0){
                SCUD_PRINT_STR("Linkable::unlink - prev not emty");
                ptn.prevObject.push_back(prev);
                prev->_unlinkSuccessor(this);
                prev=0;
            }
            lockerLinkable.unlock();
            SCUD_PRINT_STR("exit Linkable::unlink");
            
            *rc=SCUD_RC_OK;
            return ptn;
        };
        //insert this object after the specified element
        virtual SCUD_RC linkSuccessor(Linkable<TSchedulable,Tid>* link) {
            SCUD_PRINT_STR("enter Linkable::linkSuccessor");
            if(link==this){
                SCUD_PRINT_STR("exit Linkable::linkSuccessor - self referencing link");
                SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - self referencing link");
                return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
            }
            if(link==0){
                SCUD_PRINT_STR("exit Linkable::linkSuccessor - invalid param");
                SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - invalid param");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            
            if(link->hasBefore()){
                SCUD_PRINT_STR("exit Linkable::linkSuccessor - has predecessor");
                SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - has predecessor");
                return SCUD_RC_FAIL_LINK_HAS_SUCCESSOR;
            }
            if(hasAfter()){
                SCUD_PRINT_STR(" Linkable::linkSuccessor - next is NOT empty");
            }else
            {
                SCUD_RC rc=SCUD_RC_OK;
                SCUD_PRINT_STR(" Linkable::linkSuccessor - next is empty");
                
                if(link->_linkPredecessor(this)==SCUD_RC_OK){
                    lockerLinkable.lock();
                    next=link;
                    lockerLinkable.unlock();
                    SCUD_PRINT_STR("exit Linkable::linkSuccessor - OK");
                }else{
                    rc=SCUD_RC_LINK_FAILED;
                    SCUD_PRINT_STR("exit Linkable::linkSuccessor - link failed");
                    SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - link failed");
                }
                
                return rc;
            }
            SCUD_PRINT_STR("exit Linkable::linkSuccessor - link exists");
            SCUD_THROW_EXCEPTION("exit Linkable::linkSuccessor - link exists");
            return SCUD_RC_FAIL_LINK_EXISTS;
        };
        virtual SCUD_RC linkPredecessor(Linkable<TSchedulable,Tid>* link){
            SCUD_PRINT_STR("enter Linkable::linkPredecessor - link exists");
            if(link==this){
                SCUD_PRINT_STR("exit Linkable::linkPredecessor - self referencing link");
                SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - self referencing link");
                return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
            }
            if(link==0){
                SCUD_PRINT_STR("exit Linkable::linkPredecessor - invalid link");
                SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - invalid link");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            if(link->hasAfter()){
                SCUD_PRINT_STR("exit Linkable::linkPredecessor - has successor");
                SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - has successor");
                return SCUD_RC_FAIL_LINK_HAS_PREDECESSOR;
            }
            if(link->hasBefore()){
                
            }else
                //if(prev==0)
            {
                SCUD_PRINT_STR(" Linkable::linkSuccessor - prev is empty");
                lockerLinkable.lock();
                prev=link;
                prev->_linkSuccessor(this);
                lockerLinkable.unlock();
                SCUD_PRINT_STR("exit Linkable::linkPredecessor - OK");
                return SCUD_RC_OK;
            }
            SCUD_PRINT_STR("exit Linkable::linkPredecessor - link exists");
            SCUD_THROW_EXCEPTION("exit Linkable::linkPredecessor - link exists");
            return SCUD_RC_FAIL_LINK_EXISTS;
        };
        
    };
    /*
     -----------------------------------------------------------------------------
     NULL OBJECT
     
     -----------------------------------------------------------------------------
     
     */
    template<typename TSchedulable,typename Tid> class LinkableNull :public Linkable<TSchedulable,Tid>{
        //Locker lockerNull;
    protected:
        SCUD_RC _simulatePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            qu.schParam=-1;
#ifdef SCUD_WFQ_AVAILABLE
            qu.weight=-1;
            qu.prevWeight=-1;
#endif
            SCUD_RC r=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            return r;
        }
        SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu, typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI){
            SCUD_RC retcode=SCUD_RC_OK;
            SCUD_PRINT_STR("enter LinkableNull::_pull");
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            if(p && _canPull())
            {
                SCUD_PRINT_STR("+LinkableNull::_pull");
                retcode=p->_pull(qu,uCI);
                SCUD_PRINT_STR("-LinkableNull::_pull");
            }else{
                retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            }
            
            SCUD_PRINT_STR("exit LinkableNull::_pull");
            return retcode;
        }
        void _signalAvailability(bool canPull,long long countAvailable, float weight,char priority){
            SCUD_PRINT_STR("LinkableNull::_signalAvailability");
        }
        bool _canPull(){
            bool res=false;
            
            SCUD_PRINT_STR("enter LinkableNull::_canPull");
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            if(p){
                res= p->canPull();
            }
            SCUD_PRINT_STR("exit LinkableNull::_canPull");
            return res;
        }
    public:
        LinkableNull(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="Null";
#endif
        }
        LinkableNull(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="Null";
#endif
            this->setId(tid);
        };
        SCUD_RC push(TSchedulable sch, long long schedulingParam){
            SCUD_PRINT_STR("enter LinkableNull::push");
            this->processOnPush(sch, schedulingParam);
            SCUD_PRINT_STR("enter LinkableNull::push");
            return SCUD_RC_OK;
        }
        SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC retcode=SCUD_RC_OK;
            SCUD_PRINT_STR("enter LinkableNull::pull");
            typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI;
            retcode= this->_pull(qu,uCI);
            this->processOnPull(qu.scheduled, qu.schParam);
            SCUD_PRINT_STR("exit LinkableNull::pull");
            return retcode;
        }
        bool canPull(){
            SCUD_PRINT_STR("enter LinkableNull::canPull");
            //this->lockerLinkable.lock();
            bool res=this->_canPull();
            //this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableNull::canPull");
            return res;
        }
    };
    /*
     ---------------------------------------------------------------------------
     DROPPER
     ---------------------------------------------------------------------------
     */
    template<typename TSchedulable,typename Tid> class LinkableDropper :public Linkable<TSchedulable,Tid>{
        float randoms[SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT];
        int currentRandom;
        float droppingProbability;
        SCRng rng;
    protected:
        SCUD_RC _simulatePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC r=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            if(expectedToDrop(qu.scheduled, qu.schParam)){
                qu.schParam=-1;
                
            }else{
                this->lockerLinkable.lock();
                r= this->prev->_simulatePull(qu);
#ifdef SCUD_WFQ_AVAILABLE
                if(this->scp.weight<SCUD_WFQ_MIN_POSSIBLE_WEIGHT){
                    SCUD_PRINT_STR("WARNING: weight is less than possible minimum");
                }
                qu.weight=this->scp.weight;
                qu.prevWeight=this->scp.prevWeight;
#endif
                this->lockerLinkable.unlock();
            }
            return r;
        }
        bool _expectedToDrop(TSchedulable sch, long long schedulingParam){
            bool res=true;
            this->lockerLinkable.lock();
            if(randoms[currentRandom]>droppingProbability){
                res=false;
            }
            this->lockerLinkable.unlock();
            return res;
        }
        bool _shouldDrop(TSchedulable& sch, long long schedulingParam){
            bool res=true;
            float r=rng.randomFloat();
            this->lockerLinkable.lock();
            currentRandom=(currentRandom+1)%SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT ;
            if(randoms[currentRandom]>droppingProbability){
                res=false;
            }
            randoms[currentRandom]=r;
            this->lockerLinkable.unlock();
            return res;
        }
        SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>*  link,typename Linkable<TSchedulable,Tid>::SchedulingProperties scps){
            SCUD_PRINT_STR("LinkablePass::_propagateSchedulingProperties");
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            this->lockerLinkable.unlock();
            if(n){
                n->_propagateSchedulingProperties(link,scps);
            }
            return SCUD_RC_OK;
        };
        SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu, typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI){
            SCUD_RC retcode=SCUD_RC_OK;
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            if(p && canPull())
            {
                retcode=p->_pull(qu,uCI);
            }else {
                retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            }
            
            return retcode;
        }
        void _signalAvailability(bool canPull, long long countAvailable,float weight,char priority){
            if(canPull){
                this->lockerLinkable.lock();
                Linkable<TSchedulable,Tid>* n=this->next;
                this->lockerLinkable.unlock();
                TSchedulable sch;
                long long schedulingParam = 0;
                if(n){
                    if(expectedToDrop(sch, schedulingParam)){
                        n->_signalAvailability(false, countAvailable,weight,priority);
                    }
                }
            }else{
                
            }
        };

        void initializeRandomSamples(){
            currentRandom=0;
            droppingProbability=-1;
            for (int i=0;i<SCUD_DROPPER_RANDOM_NUMBERS_AMOUNT;++i) {
                randoms[i]=rng.randomFloat();
            }
            
        }
    public:
        LinkableDropper(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="Dropper";
#endif
            this->setId(tid);
            initializeRandomSamples();
        };
        LinkableDropper(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="Dropper";
#endif
            this->setId(this);
            initializeRandomSamples();
        };
        
        //Probablity of dropping object; must be in range [0..1]
        SCUD_RC setDroppingProbability(float probability){
            if(probability<0||probability>1){
                return SCUD_RC_FAIL_INVALID_PARAM;
            }else{
                this->lockerLinkable.lock();
                this->droppingProbability=probability;
                this->lockerLinkable.unlock();
                return SCUD_RC_OK;
            }
        }
        SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC retcode=SCUD_RC_OK;
            SCUD_PRINT_STR("enter LinkableDropper::Pull");
            struct Linkable<TSchedulable,Tid>::Queueable pp;
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            if(p){
                retcode=p->_simulatePull(pp);
                if( retcode==SCUD_RC_OK && pp.schParam>0 && this->shouldDrop(pp.scheduled,pp.schParam)==false){
                    typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI;
                    uCI.ssciDRR.ignoreDrr=true;
                    retcode=p->_pull(qu,uCI);
                    this->processOnPull(qu.scheduled, qu.schParam);
                }else{
                    retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
                }
            }else{
                retcode=SCUD_RC_FAIL_LINK_NOT_EXISTS;
            }
            SCUD_PRINT_STR("enter LinkableDropper::Pull");
            return retcode;
        }
        SCUD_RC push(TSchedulable sch, long long schedulingParam){
            SCUD_RC retcode=SCUD_RC_OK;
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            this->lockerLinkable.unlock();
            if(n){
                if(this->shouldDrop(sch,schedulingParam)==false){
                    this->processOnPush(sch, schedulingParam);
                    retcode=n->push(sch,schedulingParam);
                }else{
                    
                }
            }else{
                retcode=SCUD_RC_FAIL_LINK_NOT_EXISTS;
            }
            
            return retcode;
        }
        virtual bool canPull(){
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            bool rc= false;
            if(p){
                rc=p->canPull();
            }
            
            return rc;
        }
        virtual bool shouldDrop(TSchedulable& sch, long long schedulingParam){
            return _shouldDrop(sch, schedulingParam);
        }
        virtual bool expectedToDrop(TSchedulable& sch, long long schedulingParam){
            return _expectedToDrop(sch, schedulingParam);
        }
        
    };
    /*
     -------------------------------------------------------------------------
     QUEUE
     -------------------------------------------------------------------------
     */
    template<typename TSchedulable,typename Tid> class LinkableQueue :public Linkable<TSchedulable,Tid>{
        SCQueue<struct Linkable<TSchedulable,Tid>::Queueable> queue;
    protected:
        long long nextSchParam;
#ifdef SCUD_WFQ_AVAILABLE
        SCUDTimestamp nextTimestamp;
#endif
        long highT;
        long lowT;
        long long defcount;
        long long drrQuantum;
        bool isEligibleForDrr(long long objsize,bool ignoreDC){
            if(ignoreDC)
                return true;
            
            if(objsize>defcount){
                return false;
            }
            
            return true;
        }
        SCUD_RC _simulatePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC rc=SCUD_RC_OK;
            this->lockerLinkable.lock();
            long long qs=queue.size();
            qu.schParam=-1;
#ifdef SCUD_WFQ_AVAILABLE
            if(this->scp.weight<SCUD_WFQ_MIN_POSSIBLE_WEIGHT){
                SCUD_PRINT_STR("WARNING: weight is less than possible minimum");
            }
            qu.weight=this->scp.weight;
            qu.prevWeight=this->scp.prevWeight;
            qu.timestamp=this->nextTimestamp;
#endif
            if(qs>this->lowT){
                qu.schParam=this->nextSchParam;
                rc=SCUD_RC_OK;
            }else{
                rc=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            }
            
            this->lockerLinkable.unlock();
            return rc;
        }
        SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu, typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI){
            SCUD_RC retcode=SCUD_RC_OK;
            struct Linkable<TSchedulable,Tid>::Queueable temp;
            temp.schParam=-1;
            this->lockerLinkable.lock();
            long long ldc=drrQuantum;
            long long qs=queue.size();
            Linkable<TSchedulable,Tid>* n=this->next;

            if(qs>0)
            {
                if(this->lowT>=0 && qs>this->lowT)
                {
                    this->queue.back(temp);
                    bool dcc=isEligibleForDrr(temp.schParam, uCI.ssciDRR.ignoreDrr);
                    if(dcc){
                        qu=temp;
                        this->queue.pop_back();
                        defcount=defcount-temp.schParam;
                        if(qs==this->lowT+1)
                        {
                            nextSchParam=-1;
#ifdef SCUD_WFQ_AVAILABLE
                            nextTimestamp=-1;
                            if(n){
                                n->_signalAvailability(false,qs-1,this->scp.weight,this->scp.priority);
                            }
#else
                            if(n){
                                n->_signalAvailability(false,qs-1,-1,this->scp.priority);
                            }
#endif
                            
                        }else{
                            this->queue.back(temp);
#ifdef SCUD_WFQ_AVAILABLE
                            nextTimestamp=temp.timestamp;
#endif
                            nextSchParam=temp.schParam;
                        }
                        
                    }else{
                        defcount=defcount+ldc;
#ifdef SCUD_WFQ_AVAILABLE
                        if(n){
                            n->_signalAvailability(false,qs,this->scp.weight,this->scp.priority);
                        }
#else
                        if(n){
                            n->_signalAvailability(false,qs,-1,this->scp.priority);
                        }
#endif
                        retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
                    }
                    this->lockerLinkable.unlock();
                    
                    
                }
                else{
                    this->lockerLinkable.unlock();
                    defcount=0;
#ifdef SCUD_WFQ_AVAILABLE
                    if(n){
                        n->_signalAvailability(false,qs,this->scp.weight,this->scp.priority);
                    }
#else
                    if(n){
                        n->_signalAvailability(false,qs,-1,this->scp.priority);
                    }
#endif
                    retcode=SCUD_RC_FAIL_LINK_UNDER_LOW_THRESHOLD;
                }
            }else{
                this->lockerLinkable.unlock();
                defcount=0;
#ifdef SCUD_WFQ_AVAILABLE
                if(n){
                    n->_signalAvailability(false,0,this->scp.weight,this->scp.priority);
                }
#else
                if(n){
                    n->_signalAvailability(false,0,-1,this->scp.priority);
                }
#endif
                retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            }
            
            return retcode;
        }
        void _signalAvailability(bool canPull,long long countAvailable,float weight,char priority){
        }
    public:
        LinkableQueue(Tid tid):lowT(SCUD_QUEUE_DEFAULT_LOW_THRESHOLD),highT(SCUD_QUEUE_DEFAULT_HIGH_THRESHOLD){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="Queue";
#endif
            this->setId(tid);
            this->defcount=SCUD_DRR_QUANTUM;
            this->drrQuantum=SCUD_DRR_QUANTUM;
#ifdef SCUD_WFQ_AVAILABLE
            this->nextTimestamp=0;
#endif
            this->nextSchParam=-1;
        };
        LinkableQueue():lowT(SCUD_QUEUE_DEFAULT_LOW_THRESHOLD),highT(SCUD_QUEUE_DEFAULT_HIGH_THRESHOLD){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="Queue";
#endif
            this->setId(this);
            this->defcount=SCUD_DRR_QUANTUM;
            this->drrQuantum=SCUD_DRR_QUANTUM;
#ifdef SCUD_WFQ_AVAILABLE
            this->nextTimestamp=0;
#endif
            this->nextSchParam=-1;
        };
        SCUD_RC setHighThreshold(long high){
            if(high<0)
                return SCUD_RC_FAIL_INVALID_PARAM;
            this->lockerLinkable.lock();
            if(lowT>high && high>0){
                this->lockerLinkable.unlock();
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            highT=high;
            this->lockerLinkable.unlock();
            return SCUD_RC_OK;
        };
        SCUD_RC setLowThreshold(long low){
            if(low<0)
                return SCUD_RC_FAIL_INVALID_PARAM;
            this->lockerLinkable.lock();
            if(low>highT && highT>0){
                this->lockerLinkable.unlock();
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            lowT=low;
            this->lockerLinkable.unlock();
            return SCUD_RC_OK;
        };
        SCUD_RC setDRRQuantum(long quantum){
            SCUD_PRINT_STR("enter LinkableQueue::setDRRQuantum ");
            if(quantum<1){
                SCUD_PRINT_STR("exit LinkableQueue::setDRRQuantum - invalid DRR quantum");
                return SCUD_RC_FAIL_INVALID_DRR_QUANTUM;
            }
            this->lockerLinkable.lock();
            drrQuantum=quantum;
            this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableQueue::setDRRQuantum ");
            return SCUD_RC_OK;
        }
        SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC retcode=SCUD_RC_OK;
            typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI;
            uCI.ssciDRR.ignoreDrr=true;
            retcode= this->_pull(qu,uCI);
            this->processOnPull(qu.scheduled, qu.schParam);
            return retcode;
        }
        SCUD_RC push(TSchedulable sch, long long schedulingParam){
            SCUD_PRINT_STR("enter LinkableQueue::push ");
            if(schedulingParam<1){
                SCUD_PRINT_STR("exit LinkableQueue::push - invalid scheduling param, must be strictly positive");
                return SCUD_RC_FAIL_INVALID_SCHEDULING_PARAM;
            }
            SCUD_RC res=SCUD_RC_OK;
            
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            long long qs=queue.size();
            long hT=this->highT,lT=this->lowT;
            if(hT>0 && qs+1<hT)
            {
                struct Linkable<TSchedulable,Tid>::Queueable q;
                q.scheduled=sch;
#ifdef SCUD_WFQ_AVAILABLE
                q.timestamp=SCHelper::newCount();//SCTime::getCurrentTime();
                q.weight=this->scp.weight;
                q.prevWeight=this->scp.prevWeight;
#endif
                q.schParam=schedulingParam;
                this->queue.push_front(q);
                this->lockerLinkable.unlock();
                if(n && qs==lT ){
                    nextSchParam=schedulingParam;
#ifdef SCUD_WFQ_AVAILABLE
                    n->_signalAvailability(true,qs+1,this->scp.weight,this->scp.priority);
#else
                    n->_signalAvailability(true,qs+1,-1,this->scp.priority);
#endif
                }
                this->processOnPush(sch, schedulingParam);

            }else 
            {
                this->lockerLinkable.unlock();
                res=SCUD_RC_FAIL_LINK_ABOVE_HIGH_THRESHOLD;
            }
            
            SCUD_PRINT_STR("exit LinkableQueue::push ");
            return res;
        }
        virtual SCUD_RC empty(){
            this->lockerLinkable.lock();
            queue.clear();
            this->lockerLinkable.unlock();
            return SCUD_RC_OK;
        };
        bool canPull(){
            bool res=false;
            this->lockerLinkable.lock();
            long long qs=queue.size();
            if(qs>this->lowT){
                res=true;
            }
            this->lockerLinkable.unlock();
            return res;
        }
        long long size(){
            this->lockerLinkable.lock();
            long long _size=queue.size();
            this->lockerLinkable.unlock();
            return _size;
        }
    };
    /*
     -----------------------------------------------------------------------------
     PASS-THROUGH LINKABLE OBJECT
     
     -----------------------------------------------------------------------------
     
     */
    template<typename TSchedulable,typename Tid> class LinkablePass :public Linkable<TSchedulable,Tid>{
    protected:
        SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu, typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI){
            SCUD_RC retcode=SCUD_RC_OK;
            SCUD_PRINT_STR("enter LinkablePass::_pull");
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            if(p && _canPull())
            {
                SCUD_PRINT_STR("+LinkablePass::_pull");
                retcode=p->_pull(qu,uCI);
                SCUD_PRINT_STR("-LinkablePass::_pull");
            }else{
                retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            }
            
            SCUD_PRINT_STR("exit LinkablePass::_pull");
            return retcode;
        }
        SCUD_RC _simulatePull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC r=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            this->lockerLinkable.lock();
            if(this->prev){
                r= this->prev->_simulatePull(qu);
            }
#ifdef SCUD_WFQ_AVAILABLE
            if(this->scp.weight<SCUD_WFQ_MIN_POSSIBLE_WEIGHT){
                SCUD_PRINT_STR("WARNING: weight is less than possible minimum");
            }
            qu.weight=this->scp.weight;
            qu.prevWeight=this->scp.prevWeight;
#endif
            this->lockerLinkable.unlock();
            return r;
        }
        SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>*  link,typename Linkable<TSchedulable,Tid>::SchedulingProperties scps){
            SCUD_PRINT_STR("LinkablePass::_propagateSchedulingProperties");
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            this->lockerLinkable.unlock();
            if(n){
                n->_propagateSchedulingProperties(link,scps);
            }
            return SCUD_RC_OK;
        };
        void _signalAvailability(bool canPull, long long countAvailable, float weight,char priority){
            SCUD_PRINT_STR("LinkablePass::_signalAvailability");
            
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            this->lockerLinkable.unlock();
            if(n){
                n->_signalAvailability(canPull,countAvailable,weight,priority);
            }
        }
        bool _canPull(){
            bool res=false;
            
            SCUD_PRINT_STR("enter LinkablePass::_canPull");
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* p=this->prev;
            this->lockerLinkable.unlock();
            if(p){
                res= p->canPull();
            }
            SCUD_PRINT_STR("exit LinkablePass::_canPull");
            return res;
        }
    public:
        LinkablePass(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="PassThrough";
#endif
            this->setId(tid);
        };
        LinkablePass(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="PassThrough";
#endif
            this->setId(this);
        }
        SCUD_RC push(TSchedulable sch, long long schedulingParam){
            SCUD_RC retcode=SCUD_RC_OK;
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* n=this->next;
            this->lockerLinkable.unlock();
            if(n){
                this->processOnPush(sch, schedulingParam);
                retcode=n->push(sch,schedulingParam);
                
            }else{
                retcode=SCUD_RC_FAIL_LINK_NOT_EXISTS;
            }
            
            return retcode;
        }
        SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC retcode=SCUD_RC_OK;
            SCUD_PRINT_STR("enter LinkablePass::pull");
            typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI;
            uCI.ssciDRR.ignoreDrr=true;
            retcode= this->_pull(qu,uCI);
            this->processOnPull(qu.scheduled, qu.schParam);
            SCUD_PRINT_STR("exit LinkablePass::pull");
            return retcode;
        }
        bool canPull(){
            SCUD_PRINT_STR("enter LinkablePass::canPull");
            //this->lockerLinkable.lock();
            bool res=this->_canPull();
            //this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkablePass::canPull");
            return res;
        }
        
    };
    /*
     ---------------------------------------------------------------------------------
     GENERIC SCHEDULER
     ---------------------------------------------------------------------------------
     
     */
    template<typename TSchedulable,typename Tid> class LinkableScheduler :public Linkable<TSchedulable,Tid>{
    protected:
        typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo usci;
        struct InternalContainer{
            typename Linkable<TSchedulable,Tid>::SchedulingProperties scps;
            Linkable<TSchedulable,Tid>* link;
            InternalContainer(){
                scps.priority=-1;
                link=0;
#ifdef SCUD_WFQ_SCHEDULER_AVAILABLE
                scps.weight=-1;
                Tid itsId;
#endif
            }
            InternalContainer(Tid itsId,Linkable<TSchedulable,Tid>* l, float weight, char priority){
                scps.priority=priority;
                link=l;
#ifdef SCUD_WFQ_SCHEDULER_AVAILABLE
                scps.weight=weight;
                this->itsId=itsId;
#endif
            }
        };
        SCMap<Tid,InternalContainer> id2prepended ;
        virtual Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded)=0;
        virtual void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){};
        virtual bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority)=0;
        virtual bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link)=0;
        SCUD_RC _unlinkPredecessor(Linkable<TSchedulable,Tid>*  link){
            SCUD_PRINT_STR("enter LinkableScheduler::_unlinkPredecessor");
            if(link==0){
                SCUD_PRINT_STR("exit LinkableScheduler::_unlinkPredecessor - invalid param");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            if(link==this){
                SCUD_PRINT_STR("exit LinkableScheduler::_unlinkPredecessor - self referencing");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            
            this->lockerLinkable.lock();
            
            this->prev=0;
            
            Tid linkId=link->getId();
            
            InternalContainer ic;//=icit->second;

            bool found;
            ic=id2prepended.find(linkId,found);
            id2prepended.erase(linkId);
            this->lockerLinkable.unlock();
            if(found){
#ifdef SCUD_WFQ_SCHEDULER_AVAILABLE
                this->_releaseScheduledEntry(linkId,link,ic.scps.weight,ic.scps.priority);
#else
                this->_releaseScheduledEntry(linkId,link,-1,ic.scps.priority);
#endif
            }
            SCUD_PRINT_STR("exit LinkableScheduler::_unlinkPredecessor");
            return SCUD_RC_OK;
        };
        SCUD_RC _unlinkSuccessor(Linkable<TSchedulable,Tid>*  link){
            SCUD_PRINT_STR("enter LinkableScheduler::_unlinkSuccessor");
            if(link==0){
                SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            if(link==this){
                SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor - self referencing link");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            this->lockerLinkable.lock();
            this->next=0;
            this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableScheduler::_unlinkSuccessor - OK");
            return SCUD_RC_OK;
        };
        SCUD_RC _pull(struct Linkable<TSchedulable,Tid>::Queueable& qu, typename Linkable<TSchedulable,Tid>::uSchedulerControlInfo uCI){
            SCUD_RC retcode=SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE;
            bool objEnded=false;
            long long count=0;
            while(retcode==SCUD_RC_FAIL_LINK_NO_PACKET_AVAILABLE)
            {
                ++count;
                Linkable<TSchedulable,Tid>* link=this->calculateNextSource(objEnded);
                if(link){
                    retcode=link->_pull(qu,this->usci);
                    if( retcode==SCUD_RC_OK){
                        this->_finalizePull(link);
                    }
                    //    break;
                }else{
                    break;
                }
                this->lockerLinkable.lock();
                bool leave=count>this->id2prepended.size();
                this->lockerLinkable.unlock();
                if(leave)
                    break;
                objEnded=true;
            }
            
            return retcode;
        }
        void _signalAvailability(bool canPull, long long countAvailable,float weight,char priority){
            
        };
        SCUD_RC _linkPredecessor(Linkable<TSchedulable,Tid>* link){
            SCUD_PRINT_STR("enter LinkableScheduler::linkPredecessor");
            if(link==0){
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            if(link==this){
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
                SCUD_THROW_EXCEPTION("exit LinkableScheduler::linkPredecessor");
                return SCUD_RC_FAIL_SELF_REFERENCING_LINK_DETECTED;
            }
            if(link->hasAfter()){
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor");
                SCUD_THROW_EXCEPTION("exit LinkableScheduler::linkPredecessor");
                return SCUD_RC_FAIL_LINK_HAS_SUCCESSOR;
            }
            SCUD_RC res=SCUD_RC_OK;

            this->lockerLinkable.lock();
            Tid linkId=link->getId();
            bool found;
            id2prepended.find(linkId,found);
            if (found == false){
                //float w=link->getWeight();
                char pr=link->getPriority();
                if(this->_scheduleEntry(linkId,link,1,pr)){
                    InternalContainer ic(linkId,link,1,pr);
                    id2prepended.insert(linkId, ic);
                    this->_scheduleFinalizeEntry(linkId,link);
                    SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - OK");
                    res= SCUD_RC_OK;
                }else{
                    SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - FAIL: could not link");
                    SCUD_THROW_EXCEPTION("exit LinkableScheduler::linkPredecessor - FAIL: could not link");
                    res= SCUD_RC_LINK_FAILED;
                }
            }else{
                SCUD_PRINT_STR("exit LinkableScheduler::linkPredecessor - link exists");
                res= SCUD_RC_FAIL_LINK_EXISTS;
            }
            this->lockerLinkable.unlock();
            return res;
        };
        virtual void _finalizePull(Linkable<TSchedulable,Tid>*){
            
        }
        virtual ~LinkableScheduler(){

        };
        LinkableScheduler(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="GenericScheduler";
#endif
            usci.ssciDRR.ignoreDrr=true;
        };
    public:
        
        SCUD_RC pull(struct Linkable<TSchedulable,Tid>::Queueable& qu){
            SCUD_RC retcode=this->_pull(qu,this->usci);
            this->processOnPull(qu.scheduled, qu.schParam);
            return retcode;
        }
        SCUD_RC linkPredecessor(Linkable<TSchedulable,Tid>* link){
            SCUD_RC rc=this->_linkPredecessor(link);
            if( rc==SCUD_RC_OK){
                link->_linkSuccessor(this);
            }
            return rc;
        }
        typename Linkable<TSchedulable,Tid>::LinkedObjectsTuple unlink(SCUD_RC* rc){
            typename Linkable<TSchedulable,Tid>::LinkedObjectsTuple ptn(0,this,0);
            SCUD_PRINT_STR("enter LinkableScheduler::unlink");
            this->lockerLinkable.lock();
            for (id2prepended.resetIterator(); id2prepended.isExgausted()==false;id2prepended.promoteIterator() ){
                InternalContainer ic=id2prepended.getCurrentContent();
                Tid t=id2prepended.getCurrentId();
                ptn.prevObject.push_back(ic.link);
                ic.link->_unlinkSuccessor(this);
#ifdef SCUD_WFQ_SCHEDULER_AVAILABLE
                this->_releaseScheduledEntry(t,ic.link,ic.scps.weight,ic.scps.priority);
#else
                this->_releaseScheduledEntry(t,ic.link,-1,ic.scps.priority);
#endif
            }
            id2prepended.clear();
            ptn.nextObject=this->next;
            Linkable<TSchedulable,Tid>* n=this->next;
            this->lockerLinkable.unlock();
            if(n){
                n->_unlinkPredecessor(this);
            }
            
            SCUD_PRINT_STR("exit LinkableScheduler::unlink");
            *rc=SCUD_RC_OK;
            return ptn;
            ;
        };
        bool hasBefore(){

            return false;
        }
        SCUD_RC push(TSchedulable sch, long long schedulingParam){
            return SCUD_RC_OK;
        }
        
    };
    /*
     -----------------------------------------------------------------------------
     NAIVE ROUND ROBIN
     
     -----------------------------------------------------------------------------
     */
    template<typename TSchedulable,typename Tid> class  LinkableSchedulerNaiveRR:public LinkableScheduler<TSchedulable,Tid>{
    protected:
        void _signalAvailability(bool canPull, long long countAvailable, float weight,char priority){
            if(canPull){
            }
            else{
                this->lockerLinkable.lock();
                this->id2prepended.promoteIteratorSafely();
                this->lockerLinkable.unlock();
            }
        }
        bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
            this->id2prepended.resetIterator();
            return true;
        };
        bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
            this->id2prepended.resetIterator();
            return true;
        }
        void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
        }
        Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded){
            Linkable<TSchedulable,Tid>* l=0;
            this->lockerLinkable.lock();
            long long entriesCount=this->id2prepended.size();
            if(entriesCount==0){
                this->lockerLinkable.unlock();
                return l;
            }
            if(entriesCount==1){

                this->id2prepended.resetIterator();
                //this->rit=this->id2prepended.begin();
                l=this->id2prepended.getCurrentContent().link;
                //l=this->rit->second.link;
                if(l && l->canPull()){
                    
                }else{
                    this->lockerLinkable.unlock();
                    return 0;
                }
            }else
            {
                long long count=0;
                while(1)
                {
                    l=this->id2prepended.getCurrentContent().link;
                    //++(this->rit);
                    this->id2prepended.promoteIteratorSafely();
                    ++count;
                    if(l && l->canPull())
                        break;
                    if(count>=entriesCount){
                        l=0;
                        break;
                    }
                }
            }
            this->lockerLinkable.unlock();
            return l;
        };
    public:
        LinkableSchedulerNaiveRR(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerNaiveRR";
#endif
            this->setId(tid);
            this->id2prepended.resetIterator();
        };
        LinkableSchedulerNaiveRR(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerNaiveRR";
#endif
            this->setId(this);
            this->id2prepended.resetIterator();
        };
        bool canPull(){
            bool res=false;
            SCUD_PRINT_STR("enter LinkableSchedulerNaiveRR::canPull");
            this->lockerLinkable.lock();
            long long entriesCount=this->id2prepended.size();
            if(entriesCount>0){

                res=true;
            }else{
                
            }
            this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableSchedulerNaiveRR::canPull");
            return res;
        }
    };
    /*
     -----------------------------------------------------------------------------
     DEFICIT ROUND ROBIN
     
     -----------------------------------------------------------------------------
     */
    template<typename TSchedulable,typename Tid> class  LinkableSchedulerDRR:public LinkableScheduler<TSchedulable,Tid>{
    protected:
        //typename std::map<Tid,typename LinkableScheduler<TSchedulable,Tid>::InternalContainer>::iterator rit;
        bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
            
            return true;
        };
        bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){

            this->id2prepended.resetIterator();
            return true;
        }
        void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){

        }
        void _signalAvailability(bool canPull, long long countAvailable,float weight,char priority){
            if(canPull){
                
            }else{
                this->lockerLinkable.lock();
                this->id2prepended.promoteIteratorSafely();
                this->lockerLinkable.unlock();
            }
        };
        Linkable<TSchedulable,Tid>* calculateNextSource(bool pktsEnded){
            Linkable<TSchedulable,Tid>* l=0;
            this->lockerLinkable.lock();
            long long entriesCount=this->id2prepended.size();
            if(entriesCount==0){
                this->lockerLinkable.unlock();
                return l;
            }
            if(entriesCount==1)
            {
                this->id2prepended.resetIterator();

                l=this->id2prepended.getCurrentContent().link;

                if(l && l->canPull()){
                    
                }else{
                    this->lockerLinkable.unlock();
                    return 0;
                }
            }
            else
            {
                long long count=0;
                while(1)
                {
                    l=this->id2prepended.getCurrentContent().link;
                    
                    if(l){
                        if(l->canPull()){
                            break;
                        }else{
                            this->id2prepended.promoteIteratorSafely();
                            ++count;
                        }
                        
                    }else{
                        break;
                    }
                    if(count>=entriesCount){
                        l=0;
                        break;
                    }
                }
            }
            this->lockerLinkable.unlock();
            return l;
        };
    public:
        LinkableSchedulerDRR(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerDeficitRR";
#endif
            this->setId(tid);
            this->id2prepended.resetIterator();
            this->usci.ssciDRR.ignoreDrr=false;
        };
        LinkableSchedulerDRR(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerDeficitRR";
#endif
            this->setId(this);
            this->id2prepended.resetIterator();
            //this->rit=this->id2prepended.begin();
            this->usci.ssciDRR.ignoreDrr=false;
        };
        bool canPull(){
            bool res=false;
            SCUD_PRINT_STR("enter LinkableSchedulerDRR::canPull");
            this->lockerLinkable.lock();
            long long entriesCount=this->id2prepended.size();
            if(entriesCount>0){
                res=true;
            }else{
                
            }
            this->lockerLinkable.unlock();
            SCUD_PRINT_STR("exit LinkableSchedulerDRR::canPull");
            return res;
        }
    };
    /*
     -----------------------------------------------------------------------------
     PRIORITY SCHEDULER
     
     -----------------------------------------------------------------------------
     */
    template<typename TSchedulable,typename Tid> class  LinkableSchedulerPriority:public LinkableScheduler<TSchedulable,Tid>{
        
        Linkable<TSchedulable, Tid>* prioritizedSources[SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES];
        void resetPrioritySlot(char prio){
            if(prio<0 || prio>=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES)
                return;
            //this->lockerLinkable.lock();
            prioritizedSources[prio]=0;
            if(currentMaxPriority==prio){
                currentMaxPriority=-1;
                for(int i=SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;i>-1;--i){
                    if(prioritizedSources[prio]){
                        currentMaxPriority=i;
                        break;
                    }
                }
            }
            //this->lockerLinkable.unlock();
        }
        void resetPrioritySlotByLink(Linkable<TSchedulable, Tid>* link){
            //this->lockerLinkable.lock();
            for(int i=0;i<SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;++i)
            {
                if(prioritizedSources[i]==link)
                {
                    prioritizedSources[i]=0;
                    break;
                }
            }
            //this->lockerLinkable.unlock();
            
        }
        void resetPriorities(){
            this->lockerLinkable.lock();
            for(int i=0;i<SCUD_MAX_NUMBER_OF_AVAILABLE_PRIORITIES;++i){
                prioritizedSources[i]=0;
            }
            currentMaxPriority=-1;
            this->lockerLinkable.unlock();
        }
    protected:
        char currentMaxPriority;
        long skipEntry;
        
        Linkable<TSchedulable,Tid>* calculateNextSource(bool objsEnded){
            this->lockerLinkable.lock();
            Linkable<TSchedulable,Tid>* l=0;
            long long entriesCount=this->id2prepended.size();
            if(entriesCount==0 || currentMaxPriority<0){
                this->lockerLinkable.unlock();
                skipEntry=0;
                return l;
            }
            l=prioritizedSources[currentMaxPriority];
            //if last high-priority source was disconnected then move max priority pointer to next available entry
            if(l==0){
                skipEntry=0;
                while (currentMaxPriority>-1 && prioritizedSources[currentMaxPriority]==0){
                    l=prioritizedSources[currentMaxPriority];
                    --currentMaxPriority;
                }
            }else
                //if last high-priority source temporarily has no objects - offset max priority entry
                if(objsEnded){
                    if(skipEntry>currentMaxPriority){
                        skipEntry=0;
                    }
                    ++skipEntry;
                    while (currentMaxPriority-skipEntry>-1 ){
                        l=prioritizedSources[currentMaxPriority-skipEntry];
                        if(l)
                            break;
                        ++skipEntry;
                    }
                }
            this->lockerLinkable.unlock();
            return l;
        };
        
        void _signalAvailability(bool canPull, long long countAvailable,float weight,char priority){
            this->lockerLinkable.lock();
            skipEntry=0;
            //if objects can be pulled
            if(canPull){
                //then update current priority
                if(priority>currentMaxPriority){
                    currentMaxPriority=priority;
                }
            }else{
                if(currentMaxPriority==0){
                    currentMaxPriority=-1;
                }else{
                    //update the current priority to the first found lower than the given one
                    for(int i=priority-1;i>-1;--i){
                        if(prioritizedSources[i]){
                            currentMaxPriority=i;
                            break;
                        }
                    }
                }
                
            }
            this->lockerLinkable.unlock();
        }
        void _releaseScheduledEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char priority){
            //char p=link->getPriority();
            resetPrioritySlot(priority);
        };
        bool _scheduleFinalizeEntry(Tid linkId,Linkable<TSchedulable,Tid>* link){
            return true;
        }
        bool _scheduleEntry(Tid linkId,Linkable<TSchedulable,Tid>* link,float weight,char p){
            //char p=link->getPriority();
            if(p<0){
                SCUD_PRINT_STR("exit LinkableSchedulerPriority::_scheduleEntry - priority value less than zero; NOT scheduled");
                return false;
            }
            if(prioritizedSources[p]){
                SCUD_PRINT_STR("exit LinkableSchedulerPriority::_scheduleEntry - priority value used already; NOT scheduled");
                return false;
            }
            prioritizedSources[p]=link;
            if(p>currentMaxPriority){
                currentMaxPriority=p;
            }
            return true;
        }
        SCUD_RC _propagateSchedulingProperties(Linkable<TSchedulable,Tid>* link,typename Linkable<TSchedulable,Tid>::SchedulingProperties scps){
            if(link==0){
                SCUD_PRINT_STR("exit LinkableSchedulerPriority::_propagateSchedulingProperties - invalid param");
                return SCUD_RC_FAIL_INVALID_PARAM;
            }
            this->lockerLinkable.lock();
            if(prioritizedSources[scps.priority]!=0){
                this->lockerLinkable.unlock();
                SCUD_PRINT_STR("exit LinkableSchedulerPriority::_propagateSchedulingProperties - priority value used already");
                return SCUD_RC_FAIL_INVALID_PRIORITY;
            }
            Tid linkId=link->getId();
            bool found;
            SCUD_RC result=SCUD_RC_OK;
            struct LinkableScheduler<TSchedulable,Tid>::InternalContainer ic=this->id2prepended.find(linkId,found);
            if(ic.scps.priority>-1){
                prioritizedSources[ic.scps.priority]=0;
            }
            if(found==true){
                ic.scps.priority=scps.priority;
#ifdef SCUD_WFQ_SCHEDULER_AVAILABLE
                ic.scps.weight=scps.weight;
#endif
                ic.link=link;
                prioritizedSources[scps.priority]=link;
                this->id2prepended.setContent(linkId,ic);
                if(scps.priority>currentMaxPriority){
                    currentMaxPriority=scps.priority;
                }
            }
            this->lockerLinkable.unlock();
            result=SCUD_RC_FAIL_ILLEGAL_OP;;
            SCUD_PRINT_STR("exit LinkableSchedulerPriority::_propagateSchedulingProperties");
            return result;
        };
    public:
        LinkableSchedulerPriority(Tid tid){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerPriority";
#endif
            resetPriorities();
            this->setId(tid);
            skipEntry=0;
        };
        LinkableSchedulerPriority(){
#ifdef SCUD_DEBUG_MODE_ENABLED
            this->elementClass="SchedulerPriority";
#endif
            resetPriorities();
            this->setId(this);
            skipEntry=0;
            
        };
        
        SCUD_RC drop(){
            return SCUD_RC_OK;
        }
        bool canPull(){
            bool res=true;
            this->lockerLinkable.lock();
            if(currentMaxPriority<0||this->id2prepended.size()==0){
                res=false;
            }
            this->lockerLinkable.unlock();
            return res;
        }
    };

};

#endif /* Scud_h */
