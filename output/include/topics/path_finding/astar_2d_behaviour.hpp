#pragma once

#include "core/coredefs.hpp"

namespace pilo
{
    namespace topics
    {
        namespace path_finding
        {



#define     MC_ADC_NONE              (0x00)
#define     MC_ADC_OTH               (0x55)
#define     MC_ADC_DIAG              (0xAA)
#define     MC_ADC_ALL               (0xFF)



            enum EnumAStarCrossCornerRestrict
            {
                eNoneStrict = 0,
                eStrict = 1,
                eYesButLoose = 2,
            };

            enum EnumAStarAbsDirection
            {
                eASDirSouth = 0,
                eASDirSE = 1,
                eASDirEast = 2,
                eASDirNE = 3,
                eASDirNorth = 4,
                eASDirNW = 5,
                eASDirWest = 6,
                eASDirSW = 7,

                eASDirAbsEnumSize = 8,
            };

            enum EnumAStarRelDirection
            {
                eASDirRelUnchanged = 0,
                eASDirLeftTurn45 = 1,
                eASDirLeftTurn = 2,
                eASDirLeftTurn135 = 3,
                eASDirBackTurn = 4,
                eASDirRightTurn135 = 5,
                eASDirRightTurn = 6,
                eASDirRightTurn45 = 7,

                eASDirRelEnumSize = 8,
            };

            enum EnumAStarAutoDirChangeType
            {
                eASAutoRelDirChange = 0,
                eASAutoAbsDirChange = 1,
            };


            const static int g_dir_abs_to_rel_mapping_chart[eASDirAbsEnumSize][8] =
            {
                //index
                //0  1  2  3  4  5  6  7
                { 0, 1, 2, 3, 4, 5, 6, 7 }, //eASDirSouth
                { 7, 0, 1, 2, 3, 4, 5, 6 }, //eASDirSE
                { 6, 7, 0, 1, 2, 3, 4, 5 }, //eASDirEast
                { 5, 6, 7, 0, 1, 2, 3, 4 }, //eASDirNE
                { 4, 5, 6, 7, 0, 1, 2, 3 }, //eASDirNorth
                { 3, 4, 5, 6, 7, 0, 1, 2 }, //eASDirNW
                { 2, 3, 4, 5, 6, 7, 0, 1 }, //eASDirWest
                { 1, 2, 3, 4, 5, 6, 7, 0 }, //eASDirSW
            };

            class astar_2d_behaviour
            {
            public:
                astar_2d_behaviour()
                {
                    reset();
                }

                void reset()
                {
                    m_require_cross_corner_restrict = eNoneStrict;
                    memset(m_direction_change_penalty, 0x00, sizeof(m_direction_change_penalty));
                    m_require_direction_change_penalty = false;
                    memset(&m_auto_dir_change, 0x00, sizeof(m_auto_dir_change));
                    m_auto_dir_change[eASAutoRelDirChange] = false;
                    m_auto_dir_change[eASAutoAbsDirChange] = false;
                }

                EnumAStarCrossCornerRestrict is_cross_corner_restrict_required() const { return m_require_cross_corner_restrict;  }

                void require_cross_corner_restrict(EnumAStarCrossCornerRestrict e)
                {
                    m_require_cross_corner_restrict = e;
                }

                inline bool is_direction_change_penalty_required() const { return m_direction_change_penalty;  }

                void require_direction_change_penalty(bool bEnable)
                {
                    m_require_direction_change_penalty = bEnable;
                }

                void set_direction_change_penalty_cost(EnumAStarRelDirection dir, pilo::i32_t value)
                {
                    m_direction_change_penalty[dir] = value;
                }
                void set_direction_change_penalty_cost(bool bSet)
                {
                    if (!bSet)
                    {
                        memset(m_direction_change_penalty, 0x00, sizeof(m_direction_change_penalty));
                    }
                    else
                    {
                        set_direction_change_penalty_cost(eASDirRelUnchanged, 0);
                        set_direction_change_penalty_cost(eASDirLeftTurn45, 7);
                        set_direction_change_penalty_cost(eASDirLeftTurn, 8);
                        set_direction_change_penalty_cost(eASDirLeftTurn135, 40);
                        set_direction_change_penalty_cost(eASDirBackTurn, 50);
                        set_direction_change_penalty_cost(eASDirRightTurn135, 40);
                        set_direction_change_penalty_cost(eASDirRightTurn, 8);
                        set_direction_change_penalty_cost(eASDirRightTurn45, 7);
                    }
                }

                inline pilo::i32_t  get_direction_change_penalty_cost(EnumAStarAbsDirection current_abs_dir, unsigned int i) const
                {
                    EnumAStarRelDirection dir = relative_dir_of_node(current_abs_dir, i);
                    M_ASSERT(dir < 8 && dir >= 0);
                    return m_direction_change_penalty[(int) dir];
                }

                void set_auto_dir_change_all(pilo::u32_t rel_or_abs)
                {
                    m_auto_dir_change[rel_or_abs] = MC_ADC_ALL;
                }

                void set_auto_dir_change_all()
                {
                    m_auto_dir_change[eASAutoRelDirChange] = MC_ADC_ALL;
                    m_auto_dir_change[eASAutoAbsDirChange] = MC_ADC_ALL;                    
                }

                void set_auto_dir_change_none(pilo::u32_t rel_or_abs)
                {
                    m_auto_dir_change[rel_or_abs] = 0; 
                }

                void set_auto_dir_change_none()
                {
                    memset(&m_auto_dir_change, 0x00, sizeof(m_auto_dir_change));
                }

                void set_auto_dir_change(pilo::u32_t rel_or_abs, pilo::u32_t value)
                {
                    m_auto_dir_change[rel_or_abs] = value;
                }

                void set_auto_dir_change(pilo::u32_t rel_or_abs, pilo::u32_t which_dir, bool allowed)
                {

                    if (allowed)
                    {
                        m_auto_dir_change[rel_or_abs] = (m_auto_dir_change[rel_or_abs] | (1 << which_dir));
                    }
                    else
                    {
                        m_auto_dir_change[rel_or_abs] = (m_auto_dir_change[rel_or_abs] & (~(1 << which_dir)));
                    } 
                }

                inline bool get_auto_dir_change(pilo::u32_t rel_or_abs, pilo::u32_t which_dir) const
                {
                    return (m_auto_dir_change[rel_or_abs] & (1 << which_dir));
                }

                void require_auto_dir_change(bool req_rel, bool req_abs, bool relation_and)
                {
                    m_require_auto_dir_change[eASAutoRelDirChange] = req_rel;
                    m_require_auto_dir_change[eASAutoAbsDirChange] = req_abs;
                    m_auto_dir_change_types_relationshiop_and = relation_and;
                }

            protected:
                EnumAStarRelDirection relative_dir_of_node(EnumAStarAbsDirection current_abs_dir, unsigned int i) const
                {
                    if (i == MC_INVALID_SIZE)
                    {
                        return eASDirRelUnchanged;
                    }
                    return (EnumAStarRelDirection)g_dir_abs_to_rel_mapping_chart[current_abs_dir][i];
                }

            protected:
                EnumAStarCrossCornerRestrict        m_require_cross_corner_restrict;
                pilo::i32_t                         m_direction_change_penalty[eASDirRelEnumSize];
                bool                                m_require_direction_change_penalty;
                pilo::u32_t                         m_auto_dir_change[2];
                bool                                m_require_auto_dir_change[2];
                bool                                m_auto_dir_change_types_relationshiop_and;

            };
       


        }
    }
}