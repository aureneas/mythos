#ifndef UNROLLED_LIST_H
#define UNROLLED_LIST_H


template <typename T, unsigned NodeSize>
class unrolled_list {
    private:
        struct unrolled_list_node {
            T[NodeSize] elements;
            unsigned size;

            unrolled_list_node* prev;
            unrolled_list_node* next;
        };

        unrolled_list_node* first;
        unrolled_list_node* last;
    public:
        struct iterator {
            unrolled_list_node* node;
            unsigned index;

            bool operator!=(iterator it) {
                    return (node != it.node || index != it.index);
                }
            iterator operator++() {
                    iterator it = { node, index };
                    if (++index == size) {
                        node = node->next;
                        index = 0;
                    }
                    return it;
                }
            iterator operator--() {
                    iterator it = { node, index };
                    if (index == 0) {
                        node = node->prev;
                        index = node->size;
                    }
                    --index;
                    return it;
                }
            T operator*() {
                    return node->elements[index];
                }
        };

        unrolled_list() {
                first = nullptr;
                last = nullptr;
            }

        unrolled_list::iterator begin() {
                return { first, 0 };
            }
        unrolled_list::iterator end() {
                return { nullptr, 0 };
            }

        void push_front(T e) {
                // TODO finish later
            }
        template <class... Args> void emplace_front(Args&&... args) {
                push_front(T(args));
            }
};


#endif // UNROLLED_LIST_H
