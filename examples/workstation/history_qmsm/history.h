/*.$file${.::history.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: history.qm
* File:  ${.::history.h}
*
* This code has been generated by QM 5.0.1 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This code is covered by the following QP license:
* License #   : QPC-EVAL
* Issued to   : Institution or an individual evaluating the QP/C framework
* Framework(s): qpc
* Support ends: 2020-12-31
* Product(s)  :
* This license is available only for evaluation purposes and
* the generated code is still licensed under the terms of GPL.
* Please submit request for extension of the evaluaion period at:
* <www.state-machine.com/licensing/#RequestForm>
*/
/*.$endhead${.::history.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#ifndef HISTORY_H
#define HISTORY_H

enum ToastOvenSignals {
    OPEN_SIG = Q_USER_SIG,
    CLOSE_SIG,
    TOAST_SIG,
    BAKE_SIG,
    OFF_SIG,
    TERMINATE_SIG /* terminate the application */
};

extern QHsm * const the_oven; /* opaque pointer to the oven MSM */

/*.$declare${SMs::ToastOven_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${SMs::ToastOven_ctor} ..................................................*/
void ToastOven_ctor(void);
/*.$enddecl${SMs::ToastOven_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#endif /* HISTORY_H */
