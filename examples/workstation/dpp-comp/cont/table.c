/*.$file${Cont::.::table.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: dpp.qm
* File:  ${Cont::.::table.c}
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
/*.$endhead${Cont::.::table.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/*.$declare${Cont::Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::Table} ..........................................................*/
typedef struct Table {
/* protected: */
    QActive super;

/* public: */

/* private: */
    Philo philo[N_PHILO];
    uint8_t fork[N_PHILO];
    uint8_t isHungry[N_PHILO];
} Table;
extern Table Table_inst;

/* protected: */
static QState Table_initial(Table * const me, QEvt const * const e);
static QState Table_active(Table * const me, QEvt const * const e);
static QState Table_serving(Table * const me, QEvt const * const e);
static QState Table_paused(Table * const me, QEvt const * const e);
/*.$enddecl${Cont::Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#define RIGHT(n_) ((uint8_t)(((n_) + (N_PHILO - 1U)) % N_PHILO))
#define LEFT(n_)  ((uint8_t)(((n_) + 1U) % N_PHILO))
#define FREE      ((uint8_t)0)
#define USED      ((uint8_t)1)

/*.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*. Check for the minimum required QP version */
#if (QP_VERSION < 680U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 6.8.0 or higher required
#endif
/*.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${Cont::CompTimeEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::CompTimeEvt} ....................................................*/
/*.${Cont::CompTimeEvt::ctor} ..............................................*/
void CompTimeEvt_ctor(
    CompTimeEvt * const me,
    QActive * act,
    QHsm * comp,
    enum_t const sig,
    uint_fast8_t const tickRate)
{
    QTimeEvt_ctorX(&me->super, act, sig, tickRate);
    me->comp = comp;
}

/*.${Cont::CompTimeEvt::dispatchToComp} ....................................*/
void CompTimeEvt_dispatchToComp(CompTimeEvt const * const me) {
    QHSM_DISPATCH(me->comp, (QEvt const *)me);
}
/*.$enddef${Cont::CompTimeEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${Cont::AO_Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::AO_Table} .......................................................*/
QActive * const AO_Table = &Table_inst.super; /* opaque pointer */
/*.$enddef${Cont::AO_Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${Cont::Table_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::Table_ctor} .....................................................*/
void Table_ctor(void) {
    uint8_t n;
    Table *me = &Table_inst;

    QActive_ctor(&me->super, Q_STATE_CAST(&Table_initial));

    for (n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(&me->philo[n]);
        me->fork[n] = FREE;
        me->isHungry[n] = 0U;
    }
}
/*.$enddef${Cont::Table_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${Cont::Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${Cont::Table} ..........................................................*/
Table Table_inst;
/*.${Cont::Table::SM} ......................................................*/
static QState Table_initial(Table * const me, QEvt const * const e) {
    /*.${Cont::Table::SM::initial} */
    uint8_t n;
    (void)e; /* unused parameter */

    QS_OBJ_DICTIONARY(&Table_inst);
    QS_OBJ_DICTIONARY(&me->philo[0]);
    QS_OBJ_DICTIONARY(&me->philo[0].timeEvt);
    QS_OBJ_DICTIONARY(&me->philo[1]);
    QS_OBJ_DICTIONARY(&me->philo[1].timeEvt);
    QS_OBJ_DICTIONARY(&me->philo[2]);
    QS_OBJ_DICTIONARY(&me->philo[2].timeEvt);
    QS_OBJ_DICTIONARY(&me->philo[3]);
    QS_OBJ_DICTIONARY(&me->philo[3].timeEvt);
    QS_OBJ_DICTIONARY(&me->philo[4]);
    QS_OBJ_DICTIONARY(&me->philo[4].timeEvt);

    QS_SIG_DICTIONARY(DONE_SIG,      (void *)0); /* global signals */
    QS_SIG_DICTIONARY(EAT_SIG,       (void *)0);
    QS_SIG_DICTIONARY(PAUSE_SIG,     (void *)0);
    QS_SIG_DICTIONARY(SERVE_SIG,     (void *)0);
    QS_SIG_DICTIONARY(TEST_SIG,      (void *)0);
    QS_SIG_DICTIONARY(HUNGRY_SIG,    (void *)0);
    QS_SIG_DICTIONARY(HUNGRY_SIG,    (void *)0);
    QS_SIG_DICTIONARY(TIMEOUT_SIG,   (void *)0);

    QActive_subscribe(&me->super, PAUSE_SIG);
    QActive_subscribe(&me->super, SERVE_SIG);
    QActive_subscribe(&me->super, TEST_SIG);

    for (n = 0U; n < N_PHILO; ++n) {
        QHSM_INIT(&me->philo[n].super, (QEvt *)0);
        me->fork[n] = FREE;
        me->isHungry[n] = 0U;
        BSP_displayPhilStat(n, "thinking");
    }

    QS_FUN_DICTIONARY(&Table_active);
    QS_FUN_DICTIONARY(&Table_serving);
    QS_FUN_DICTIONARY(&Table_paused);

    return Q_TRAN(&Table_serving);
}
/*.${Cont::Table::SM::active} ..............................................*/
static QState Table_active(Table * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${Cont::Table::SM::active::TIMEOUT} */
        case TIMEOUT_SIG: {
            CompTimeEvt_dispatchToComp(Q_EVT_CAST(CompTimeEvt));
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::EAT} */
        case EAT_SIG: {
            Q_ERROR();
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::TEST} */
        case TEST_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.${Cont::Table::SM::active::serving} .....................................*/
static QState Table_serving(Table * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${Cont::Table::SM::active::serving} */
        case Q_ENTRY_SIG: {
            uint8_t n;
            for (n = 0U; n < N_PHILO; ++n) { /* give permissions to eat... */
                if ((me->isHungry[n] != 0U)
                    && (me->fork[LEFT(n)] == FREE)
                    && (me->fork[n] == FREE))
                {
                    TableEvt evt;

                    me->fork[LEFT(n)] = USED;
                    me->fork[n] = USED;

                    /* synchronoulsy dispatch EAT event to the Philo component */
                    evt.super.sig = EAT_SIG;
                    evt.philo = &me->philo[n];
                    QHSM_DISPATCH(&me->philo[n].super, (QEvt const *)&evt);

                    me->isHungry[n] = 0U;
                    BSP_displayPhilStat(n, "eating  ");
                }
            }
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::serving::HUNGRY} */
        case HUNGRY_SIG: {
            uint8_t n, m;

            /* find the index of the Philo from the event */
            n = (Q_EVT_CAST(TableEvt)->philo - &me->philo[0]);
            /* philo ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displayPhilStat(n, "hungry  ");
            m = LEFT(n);
            /*.${Cont::Table::SM::active::serving::HUNGRY::[bothfree]} */
            if ((me->fork[m] == FREE) && (me->fork[n] == FREE)) {
                TableEvt evt;
                me->fork[m] = USED;
                me->fork[n] = USED;

                /* synchronoulsy dispatch EAT event to the Philo component */
                evt.super.sig = EAT_SIG;
                evt.philo = &me->philo[n];
                QHSM_DISPATCH(&me->philo[n].super, (QEvt const *)&evt);

                BSP_displayPhilStat(n, "eating  ");
                status_ = Q_HANDLED();
            }
            /*.${Cont::Table::SM::active::serving::HUNGRY::[else]} */
            else {
                me->isHungry[n] = 1U;
                status_ = Q_HANDLED();
            }
            break;
        }
        /*.${Cont::Table::SM::active::serving::DONE} */
        case DONE_SIG: {
            uint8_t n, m;
            TableEvt evt;

            /* find the index of the Philo from the event */
            n = (Q_EVT_CAST(TableEvt)->philo - &me->philo[0]);
            /* philo ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displayPhilStat(n, "thinking");
            m = LEFT(n);
            /* both forks of Phil[n] must be used */
            Q_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = FREE;
            me->fork[n] = FREE;
            m = RIGHT(n); /* check the right neighbor */

            if ((me->isHungry[m] != 0U) && (me->fork[m] == FREE)) {
                me->fork[n] = USED;
                me->fork[m] = USED;
                me->isHungry[m] = 0U;

                /* synchronoulsy dispatch EAT event to the Philo component */
                evt.super.sig = EAT_SIG;
                evt.philo = &me->philo[m];
                QHSM_DISPATCH(&me->philo[m].super, (QEvt const *)&evt);

                BSP_displayPhilStat(m, "eating  ");
            }
            m = LEFT(n); /* check the left neighbor */
            n = LEFT(m); /* left fork of the left neighbor */
            if ((me->isHungry[m] != 0U) && (me->fork[n] == FREE)) {
                me->fork[m] = USED;
                me->fork[n] = USED;
                me->isHungry[m] = 0U;

                /* synchronoulsy dispatch EAT event to the Philo component */
                evt.super.sig = EAT_SIG;
                evt.philo = &me->philo[m];
                QHSM_DISPATCH(&me->philo[m].super, (QEvt const *)&evt);

                BSP_displayPhilStat(m, "eating  ");
            }
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::serving::EAT} */
        case EAT_SIG: {
            Q_ERROR();
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::serving::PAUSE} */
        case PAUSE_SIG: {
            status_ = Q_TRAN(&Table_paused);
            break;
        }
        default: {
            status_ = Q_SUPER(&Table_active);
            break;
        }
    }
    return status_;
}
/*.${Cont::Table::SM::active::paused} ......................................*/
static QState Table_paused(Table * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${Cont::Table::SM::active::paused} */
        case Q_ENTRY_SIG: {
            BSP_displayPaused(1U);
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::paused} */
        case Q_EXIT_SIG: {
            BSP_displayPaused(0U);
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::paused::SERVE} */
        case SERVE_SIG: {
            status_ = Q_TRAN(&Table_serving);
            break;
        }
        /*.${Cont::Table::SM::active::paused::HUNGRY} */
        case HUNGRY_SIG: {
            uint8_t n;

            /* find the index of the Philo from the event */
            n = (Q_EVT_CAST(TableEvt)->philo - &me->philo[0]);
            /* philo ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            me->isHungry[n] = 1U;
            BSP_displayPhilStat(n, "hungry  ");
            status_ = Q_HANDLED();
            break;
        }
        /*.${Cont::Table::SM::active::paused::DONE} */
        case DONE_SIG: {
            uint8_t n, m;

            /* find the index of the Philo from the event */
            n = (Q_EVT_CAST(TableEvt)->philo - &me->philo[0]);
            /* philo ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displayPhilStat(n, "thinking");
            m = LEFT(n);
            /* both forks of Phil[n] must be used */
            Q_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = FREE;
            me->fork[n] = FREE;
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Table_active);
            break;
        }
    }
    return status_;
}
/*.$enddef${Cont::Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
