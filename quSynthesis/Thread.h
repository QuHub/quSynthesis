#ifndef __THREAD_H__
#define __THREAD_H__
#include <Windows.h>
class CThread
{
public:
  /*
  *  Info: Default Constructor
  */
  CThread()
  { 
    m_pThreadFunc = CThread::EntryPoint; // Can call Detach() also.
  }

  /*
  *  Info: Plug Constructor
  *
  *  Use this to migrate/port existing worker threads to objects immediately
  *  Although you lose the benefits of ThreadCTOR and ThreadDTOR.
  */
  CThread(LPTHREAD_START_ROUTINE lpExternalRoutine)
  {
    Attach(lpExternalRoutine);
  }

  /*
  *  Info: Default Destructor
  *
  *  I think it is wise to destroy the thread even if it is running,
  *  when the main thread reaches here.
  */
  ~CThread()
  {
    if ( m_ThreadCtx.m_hThread )
      Stop(true);
  }
  /*
  *  Info: Starts the thread.
  *  
  *  This function starts the thread pointed by m_pThreadFunc with default attributes
  */
  DWORD Start( void* arg = NULL )
  {
    m_ThreadCtx.m_pUserData = arg;
    m_ThreadCtx.m_hThread = CreateThread(NULL, 0, m_pThreadFunc, this, 0, &m_ThreadCtx.m_dwTID);
    m_ThreadCtx.m_dwExitCode = (DWORD)-1;

    return GetLastError();
  }

  /*
  *  Info: Stops the thread.
  *  
  *  This function stops the current thread. 
  *  We can force kill a thread which results in a TerminateThread.
  */
  DWORD Stop ( bool bForceKill = false )
  {
    if ( m_ThreadCtx.m_hThread )
    {
      GetExitCodeThread(m_ThreadCtx.m_hThread, &m_ThreadCtx.m_dwExitCode);

      if ( m_ThreadCtx.m_dwExitCode == STILL_ACTIVE && bForceKill )
        TerminateThread(m_ThreadCtx.m_hThread, DWORD(-1));

      m_ThreadCtx.m_hThread = NULL;
    }

    return m_ThreadCtx.m_dwExitCode;
  }

  /*
  *  Info: Starts the thread.
  *  
  *  This function starts the thread pointed by m_pThreadFunc with default attributes
  */
  DWORD GetExitCode() const 
  { 
    if ( m_ThreadCtx.m_hThread )
      GetExitCodeThread(m_ThreadCtx.m_hThread, (LPDWORD)&m_ThreadCtx.m_dwExitCode);
    return m_ThreadCtx.m_dwExitCode;
  }

  /*
  *  Info: Attaches a Thread Function
  *  
  *  Used primarily for porting but can serve in developing generic thread objects
  */
  void Attach( LPTHREAD_START_ROUTINE lpThreadFunc ){
    m_pThreadFunc = lpThreadFunc;
  }

  /*
  *  Info: Detaches the Attached Thread Function
  *  
  *  Detaches the Attached Thread Function, If any.
  *  by resetting the thread function pointer to EntryPoint1
  */
  void  Detach( void ){
    m_pThreadFunc = CThread::EntryPoint; 
  }

protected:

  /*
  *  Info: DONT override this method.
  *  
  *  This function is like a standard template. 
  *  Override if you are sure of what you are doing.
  */
  static DWORD WINAPI EntryPoint( LPVOID pArg)
  {
    CThread *pParent = reinterpret_cast<CThread*>(pArg);

    pParent->ThreadCtor();

    pParent->Run( pParent->m_ThreadCtx.m_pUserData );

    pParent->ThreadDtor();

    return STILL_ACTIVE;
  }

  /*
  *  Info: Override this method.
  *  
  *  This function should contain the body/code of your thread.
  *  Notice the signature is similar to that of any worker thread function
  *  except for the calling convention.
  */
  virtual DWORD Run( LPVOID /* arg */ )
  { return m_ThreadCtx.m_dwExitCode; }

  /*
  *  Info: Constructor-like function. 
  *  
  *  Will be called by EntryPoint before executing the thread body.
  *  Override this function to provide your extra initialization.
  *
  *  NOTE: do not confuse it with the classes constructor
  */
  virtual void ThreadCtor(){  }

  /*
  *  Info: Destructor-like function. 
  *  
  *  Will be called by EntryPoint after executing the thread body.
  *  Override this function to provide your extra destruction.
  *
  *  NOTE: do not confuse it with the classes constructor
  */
  virtual void ThreadDtor(){  }

private:
  /*
  *  Info: Thread Context Inner Class
  *  
  *  Every thread object needs to be associated with a set of values.
  *  like UserData Pointer, Handle, Thread ID etc.
  *  
  *  NOTE: This class can be enhanced to varying functionalities
  *        eg.,
  *              * Members to hold StackSize
  *              * SECURITY_ATTRIBUTES member.
  */
  class CThreadContext
  {
  public:
    CThreadContext(){
      memset(this, 0, sizeof(this));
    }

    /*
    *  Attributes Section
    */
  public:
    HANDLE m_hThread;                   //  The Thread Handle
    DWORD  m_dwTID;                     //  The Thread ID
    LPVOID m_pUserData;                     //  The user data pointer
    LPVOID m_pParent;                   //  The this pointer of the parent CThread object
    DWORD  m_dwExitCode;                //  The Exit Code of the thread
  };

  /*
  *  Attributes Section
  */
protected:
  /*
  *  Info: Members of CThread
  */
  CThreadContext          m_ThreadCtx;    //  The Thread Context member
  LPTHREAD_START_ROUTINE  m_pThreadFunc;  //  The Worker Thread Function Pointer
};

#endif //__THREAD_H__