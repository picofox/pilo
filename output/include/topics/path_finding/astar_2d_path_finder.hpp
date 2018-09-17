#pragma once
#include "core/coredefs.hpp"
#include "topics/path_finding/path_2d_map.hpp"
#include "topics/path_finding/astar_2d_behaviour.hpp"
#include <queue>
#include <list>
#include <cassert>


#include "core/string/fixed_astring.hpp"
#include "core/container/fixed_bitset.hpp"

using namespace pilo::topics::path_finding;

static pilo::u32_t i_count = 0;
static pilo::u32_t i_count1 = 0;
static pilo::u32_t i_count2 = 0;
static pilo::u32_t i_count3 = 0;

namespace pilo
{
    namespace topics
    {
        namespace path_finding
        {
            const int g_asf_direnction[8][2] =
            {
                { 0,  1,  },  // south  0
                { 1,  1, }, // SE 
                { 1,  0,  }, // East 2
                { 1,  -1, }, // NE  
                { 0,  -1, }, // North 4
                { -1, -1, }, // NW 
                {-1,  0,  }, // West   6             
                {-1,  1,  }, // SW                
            };

            class abstract_astar_2d_node
            {
            public:
                abstract_astar_2d_node*     m_parent;
                pilo::i32_t                 m_f;
                pilo::i32_t                 m_g;
                pilo::i32_t                 m_h; 

                pilo::u8_t                  m_abs_dir;                
            };


            template<typename _Axis>
            class astar_2d_node : public abstract_astar_2d_node
            {
            public:           
                typedef pilo::vec2_t<_Axis> coord_type;
                coord_type  m_pos;
            public:
//                 pilo::core::string::fixed_astring<128> to_string() const
//                 {
//                     pilo::core::string::fixed_astring<128> str;
// 
//                     char buffer[128] = { 0 };
//                     pilo::core::io::string_format_output(buffer, 128, "pt_<0x%p>: @[%u:%u] f=%d g=%d h=%d, prt=<0x%p>",this, m_pos.x, m_pos.y, m_f,m_g, m_h,m_parent);                   
// 
//                     str = buffer;
//                     return str;
//                 }

                bool operator<(const astar_2d_node& o) const
                {
                    return (m_f < o.m_f);
                }

                pilo::core::string::fixed_astring<128> to_string() const
                {
                    pilo::core::string::fixed_astring<128> str;

                    char buffer[128] = { 0 };
                    pilo::core::io::string_format_output(buffer, 128, "[%u:%u] f=%d g=%d h=%d", m_pos.x, m_pos.y, m_f, m_g, m_h);

                    str = buffer;
                    return str;
                }
            };           

            template<typename _Coord>
            class closed_list_node_pos_lesser_cmp
            {
            public:
                bool operator()(const _Coord& a, const _Coord& b)
                {
                    if (a.y < b.y)
                    {
                        return true;
                    }
                    else if (a.y > b.y)
                    {
                        return false;
                    }

                    if (a.x < b.x)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            };


            template <typename _Node, typename _Open_list, size_t ROWS, size_t COLS>
            class astar_2d_path_finder
            {
            public:
                typedef _Node node_type;
                typedef typename node_type::coord_type coord_type;
                typedef _Open_list open_list_type;

                astar_2d_path_finder(pilo::topics::path_finding::abstract_path_2d_map& map) : m_map(map)
                {
                    m_behaviour = nullptr;
                    reset();
                }

                void reset()
                {
                    m_opened_list.reset();
                    m_dest = nullptr;
                    memset(m_closed_list, 0x00, sizeof(m_closed_list));
                    memset(m_nodes, 0x00, sizeof(m_nodes));
                }
                
                const node_type* get_node_by_coord(const coord_type& pos)
                {
                    if ((pos.x >= COLS || pos.y >= ROWS))
                    {
                        return nullptr;
                    }
                    return &(m_nodes[pos.y][pos.x]);
                }

                pilo::i32_t find_path(const coord_type& src, const coord_type & dst, EnumAStarAbsDirection dir, void* reachable_param)
                {
                    assert(! m_map.invalid_pos(src.x, src.y));
                    assert(! m_map.invalid_pos(dst.x, dst.y));

                    //clear internal data before every search
                    reset();

                    //init and add the start data as the 1st node in the opened node list
                    const coord_type & current_pt = src;
                    node_type* current_node = touch_new_node(current_pt, dst, 0, dir, MC_INVALID_SIZE);
                    assert(current_node != nullptr);
                    if (current_node == nullptr)
                    {                        
                        return pilo::EC_INSUFFICIENT_MEMORY;
                    }

                    //the node which we start from is always the 1st node to append
                    m_opened_list.add_node(current_node);
                    
                    //or, start our finding with the bloody complex algorithm
                    int count = 0;
                    do
                    {   //find the nearest node in the open list
                        current_node = m_opened_list.detach_lowest_cost_node();
                        if (current_node == nullptr)
                        {
                            return pilo::EC_OBJECT_NOT_FOUND;
                        }

                        //LOG_MESSAGE("## get node from ol: (%s)", current_node->to_string().c_str());

                        //may be the has already reached the dest point
                        if (current_node->m_pos.x == dst.x && current_node->m_pos.y == dst.y)
                        {
                            // where i stand, is where i would like to go, how lucky.
                            m_dest = current_node;
                            return EC_OK;
                        }

                        //shift this node to close list after got nodes around
                        shift_node_to_closed_list(current_node);     

                        //get node around current node we are processing
                        pilo::i32_t ret = obtain_nodes_around(current_node, dst, (EnumAStarAbsDirection) current_node->m_abs_dir, reachable_param);
                        if (ret != pilo::EC_OK)
                        {   //there is no more path to find
                            return ret;
                        }                         

                        count++;
                    } while (1);


                    return -1;
                }

                template<typename _Container>
                bool get_reverse_waypoints(_Container& result_container)
                {
                    if (m_dest == nullptr)
                    {
                        return false;
                    }

                    std::back_insert_iterator<_Container> inserter(result_container);

                    node_type* p = m_dest;
                    while (p != nullptr)
                    {
                        inserter = p->m_pos;

                        inserter++;
                        p = (node_type*) p->m_parent;
                    }
                    return true;
                }

                template<typename _Container>
                bool get_waypoints(_Container& result_container)
                {
                    if (m_dest == nullptr)
                    {
                        return false;
                    }

                    std::front_insert_iterator<_Container> inserter(result_container);

                    node_type* p = m_dest;
                    while (p != nullptr)
                    {
                        inserter = p->m_pos;

                        inserter++;
                        p = (node_type*)p->m_parent;
                    }
                    return true;
                }


            private:

                void shift_node_to_closed_list(node_type* node)
                {
                    m_closed_list[node->m_pos.y][node->m_pos.x] = node;
                }

                bool exist_in_closed_list(const coord_type& pos) const
                {
                    if (m_closed_list[pos.y][pos.x] != 0)
                    {
                        return true;
                    }
                    return false;
                }

                coord_type point_in_pos(const coord_type& current_pt, pilo::int_r idx)
                {
                    coord_type ret;
                    ret.x = current_pt.x + g_asf_direnction[idx][0];
                    ret.y = current_pt.y + g_asf_direnction[idx][1];  
                    return ret;
                }

                pilo::i32_t  validate_position(const coord_type& pos, void* reachable_param)
                {
                    if (m_map.invalid_pos(pos.x, pos.y))
                    {
                        //ignore node that is not in the range of map
                        return pilo::EC_INVALID_OBJECT;
                    }

                    if (!m_map.reachable(pos.x, pos.y, reachable_param))
                    {
                         // ignore pos that we can not reachable
                        return pilo::EC_INVALID_OBJECT;
                    }

                    return pilo::EC_OK;
                }

                pilo::i32_t obtain_nodes_around(node_type* current_node, const coord_type& dst, EnumAStarAbsDirection dir, void* reachable_param)
                {                    
                    pilo::i32_t ret = EC_OK;

                    bool bLastBlocked = true;

                    for (int i = 0; i < 8; i++)
                    {
                        if ((nullptr != m_behaviour) && (m_behaviour->is_cross_corner_restrict_required() != eNoneStrict) && ((i & 1) == 0) )
                        {
                            bLastBlocked = true;
                        }
                        
                        coord_type current_near_pt = point_in_pos(current_node->m_pos, i);                        

                        ret = validate_position(current_near_pt, reachable_param);
                        if (ret != pilo::EC_OK)
                        {
                            continue;
                        }                                              

                        if (exist_in_closed_list(current_near_pt))
                        {
                            //ignore node already in close list
                            continue;
                        }

                        if ((nullptr != m_behaviour) && (m_behaviour->is_cross_corner_restrict_required() != eNoneStrict) && ((i & 1) == 0))
                        {
                            bLastBlocked = false;
                        }
                        
                        //optional attribute setting for how the finder will trait the diagonal nodes
                        if ((nullptr != m_behaviour) && (m_behaviour->is_cross_corner_restrict_required() != eNoneStrict) && ((i & 1) != 0))
                        {                            
                            if (process_behaviour_restrict_cross_cornor(current_node->m_pos, i, bLastBlocked, reachable_param))
                            {
                                continue;
                            }                            
                        }
                        

                        node_type* arnode = nullptr;
                        bool bExist = m_opened_list.find_node_by_pos(current_near_pt);
                        if (! bExist)
                        {
                            // this is a new node we have just found
                            arnode = touch_new_node(current_near_pt, dst, current_node, dir, i);
                            if (arnode == nullptr)
                            {
                                return pilo::EC_INSUFFICIENT_MEMORY;
                            }
                            
                            m_opened_list.add_node(arnode);
                        }
                        else //the node around is already in the list, means we have process this point before
                        {
                            arnode = &(m_nodes[current_near_pt.y][current_near_pt.x]);
                            pilo::int_r current_gcost = calculate_g_cost(current_near_pt, current_node);

                            //to see if need to update g cost value
                            if (arnode->m_g > current_gcost)
                            {
                                arnode->m_parent = current_node;
                                arnode->m_g = current_gcost;
                                arnode->m_f = arnode->m_g + arnode->m_h;
                                m_opened_list.value_changed(arnode);
                                //arnode->m_abs_dir = (pilo::u8_t) getDir(current_node->m_pos, arnode->m_pos);
                            }
                            else
                            {
                                //there is nothing we can do, because no need to update G cost
                            }
                        }
                    } 

                    return pilo::EC_OK;
                }

                EnumAStarAbsDirection getDir(const coord_type& orig, const coord_type& surnode) const
                {
                    assert(surnode.x != orig.x || surnode.y != orig.y);

                    int xDiff = surnode.x - orig.x;
                    int yDiff = surnode.y - orig.y;

                    for (int i = 0; i < 8; i++)
                    {
                        if (g_asf_direnction[i][0] == xDiff && g_asf_direnction[i][1] == yDiff)
                        {
                            return (EnumAStarAbsDirection)i;
                        }
                    }

                    assert(false);
                    return eASDirSouth;
                }

                node_type* touch_new_node(const coord_type& current_pt, const coord_type & dst, node_type* parent, EnumAStarAbsDirection dir, unsigned int surround_node_idx)
                {
                    assert(current_pt.y < ROWS && current_pt.x < COLS);
                    node_type* current_node = &(m_nodes[current_pt.y][current_pt.x]);

                    current_node->m_abs_dir = dir;

                    current_node->m_pos.x = current_pt.x;
                    current_node->m_pos.y = current_pt.y;
                    current_node->m_h = calculate_h_cost(current_pt, dst);
                    current_node->m_g = calculate_g_cost(current_pt, parent);

                    if ((nullptr != m_behaviour) && (m_behaviour->is_direction_change_penalty_required()))
                    {
                        current_node->m_g += m_behaviour->get_direction_change_penalty_cost(dir, surround_node_idx);
                    }

                    current_node->m_f = current_node->m_h + current_node->m_g;
                    current_node->m_parent = parent;

                    

                   // LOG_MESSAGE("create new Node: %s", current_node->to_string().c_str());
                    return current_node;
                }

                pilo::int_r calculate_h_cost(const coord_type& pt, const coord_type& dst)
                {
                    pilo::int_r xDiff = dst.x - pt.x;
                    if (xDiff < 0) xDiff = 0 - xDiff;

                    pilo::int_r yDiff = dst.y - pt.y;
                    if (yDiff < 0) yDiff = 0 - yDiff;

                    return (xDiff + yDiff) * 10;
                }

                pilo::int_r calculate_g_cost(const coord_type& pt, const node_type* node)
                {          
                    if (node == nullptr)
                    {
                        return 0;
                    }

                    int g;
                    if (node->m_pos.x == pt.x || node->m_pos.y == pt.y)
                    {
                        g = 10;
                    }
                    else
                    {
                        g = 14;
                    }
                    
                    return g + node->m_g;
                }

                
            

            private: //attrs methods
                bool process_behaviour_restrict_cross_cornor(const coord_type& orig_pos, unsigned int i, bool bLastBlocked, void* reachable_param)
                { 
                    if (nullptr == m_behaviour)
                    {
                        return false;
                    }

                    if ( m_behaviour->is_cross_corner_restrict_required() == eStrict )
                    {
                        if (bLastBlocked)
                        {
                            return  true;
                        }

                        unsigned int idx_next = i + 1;
                        if (idx_next > 7)
                        {
                            idx_next = 0;
                        }

                        coord_type next_near_pt = point_in_pos(orig_pos, idx_next);
                        pilo::i32_t ret = validate_position(next_near_pt, reachable_param);
                        if (ret != EC_OK)
                        {
                            return true;
                        }
                    }
                    else if (m_behaviour->is_cross_corner_restrict_required() == eYesButLoose)
                    {
                        if (! bLastBlocked)
                        {
                            return  false;
                        }

                        unsigned int idx_next = i + 1;
                        if (idx_next > 7)
                        {
                            idx_next = 0;
                        }

                        coord_type next_near_pt = point_in_pos(orig_pos, idx_next);
                        pilo::i32_t ret = validate_position(next_near_pt, reachable_param);
                        if (ret != EC_OK)
                        {
                            return true;
                        }
                    }

                    return false;
                }            

            public:
                void set_behaviour(astar_2d_behaviour* b) { m_behaviour = b; }

            private:
                astar_2d_behaviour*                                      m_behaviour;
               
            private:
                node_type*                                               m_dest;
                pilo::topics::path_finding::abstract_path_2d_map&     m_map;
                node_type                                                m_nodes[ROWS][COLS];
                node_type*                                               m_closed_list[ROWS][COLS];
                open_list_type                                           m_opened_list;

                
            };
        }        
    }
}