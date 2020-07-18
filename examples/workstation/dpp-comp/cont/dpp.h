/*.$file${Cont::.::dpp.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: dpp.qm
* File:  ${Cont::.::dpp.h}
*
* This code has been generated by QM 5.0.2 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${Cont::.::dpp.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#ifndef DPP_H
#define DPP_H

enum DPPSignals {
    EAT_SIG = Q_USER_SIG, /* published by Table to let a philosopher eat */
    DONE_SIG,       /* published by Philosopher when done eating */
    PAUSE_SIG,      /* published by BSP to pause serving forks */
    SERVE_SIG,      /* published by BSP to serve re-start serving forks */
    TEST_SIG,       /* published by BSP to test the application */
    MAX_PUB_SIG,    /* the last published signal */

    HUNGRY_SIG,     /* posted direclty to Table from hungry Philo */
    TIMEOUT_SIG,    /* used by Philosophers for time events */
    MAX_SIG         /* the last signal */
};

enum {
    N_PHILO = 5 /* number of Philos */
};

struct Philo; /* forward declaration */

/*.$declare${Cont::CompTimeEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::CompTimeEvt} ....................................................*/
typedef struct {
/* protected: */
    QTimeEvt super;

/* public: */
    QHsm* comp;
} CompTimeEvt;

/* public: */
void CompTimeEvt_ctor(
    CompTimeEvt * const me,
    QActive * act,
    QHsm * comp,
    enum_t const sig,
    uint_fast8_t const tickRate);
void CompTimeEvt_dispatchToComp(CompTimeEvt const * const me);
/*.$enddecl${Cont::CompTimeEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$declare${Comp::Philo} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Comp::Philo} ..........................................................*/
typedef struct {
/* protected: */
    QHsm super;

/* private: */
    CompTimeEvt timeEvt;
} Philo;

/* public: */
void Philo_ctor(Philo * const me);

/* protected: */
QState Philo_initial(Philo * const me, QEvt const * const e);
QState Philo_thinking(Philo * const me, QEvt const * const e);
QState Philo_hungry(Philo * const me, QEvt const * const e);
QState Philo_eating(Philo * const me, QEvt const * const e);
/*.$enddecl${Comp::Philo} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$declare${Cont::TableEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::TableEvt} .......................................................*/
typedef struct {
/* protected: */
    QEvt super;

/* public: */
    Philo* philo;
} TableEvt;
/*.$enddecl${Cont::TableEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$declare${Cont::Table_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::Table_ctor} .....................................................*/
void Table_ctor(void);
/*.$enddecl${Cont::Table_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$declare${Cont::AO_Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
extern QActive * const AO_Table;
/*.$enddecl${Cont::AO_Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#endif /* DPP_H */
