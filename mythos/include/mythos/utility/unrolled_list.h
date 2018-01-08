#ifndef UNROLLED_LIST_H
#define UNROLLED_LIST_H


template <typename T, unsigned NodeSize>
class unrolled_list {
    private:
        struct unrolled_list_node {
            T elements[NodeSize];
            unsigned size;

            unrolled_list_node* prev;
            unrolled_list_node* next;
        };

        unrolled_list_node* first;
        unrolled_list_node* last;

        void make_node_before(unrolled_list_node* old) {
                unrolled_list_node* node = new unrolled_list_node();
                node->size = 0;
                node->next = old;
                if (old) {
                    node->prev = old->prev;
                    node->prev->next = node;
                    old->prev = node;
                } else {
                    node->prev = last;
                    if (last) {
                        last->next = node;
                        last = node;
                    } else {
                        first = node;
                        last = node;
                    }
                }
            }
        void make_node_after(unrolled_list_node* old) {
                unrolled_list_node* node = new unrolled_list_node();
                node->size = 0;
                node->prev = old;
                if (old) {
                    node->next = old->next;
                    node->next->prev = node;
                    old->next = node;
                } else {
                    node->next = first;
                    if (first) {
                        first->prev = node;
                        first = node;
                    } else {
                        first = node;
                        last = node;
                    }
                }
            }
        void divide_node(unrolled_list_node* node) {
                make_node_after(node);
                unsigned half = NodeSize / 2;
                if (half < node->size) {
                    node->next->size = node->size - half;
                    node->size = half;
                    for (unsigned i = node->next->size - 1; i >= 0; --i)
                        node->next->elements[i] = node->elements[half + i];
                }
            }
        void shift_elements(unrolled_list_node* node, unsigned index) {
                for (unsigned i = index; i < node->size; ++i)
                    node->elements[i+1] = node->elements[i];
            }
        void sort(bool (*comp)(T, T), unrolled_list_node* n1, unrolled_list_node* n2) {
                if (n1 == n2) {
                    std::sort(n1->elements, n1->elements + n1->size, comp);
                    return;
                }
                sort(comp, n1, n1);
                sort(comp, n1->next, n2);
            }
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
                if (first->size == NodeSize)
                    make_node_before(first);
                else
                    shift_elements(first, 0);
                first->elements[0] = e;
            }
        void push_back(T e) {
                if (last->size == NodeSize)
                    make_node_before(nullptr);
                last->elements[last->size++] = e;
            }
        void insert(iterator prev) {
                if (prev.node->size == NodeSize) {
                    if (prev.node->next->size == NodeSize) {

                    }
                }
            }
        template <class... Args> void emplace_front(Args&&... args) {
                push_front(T(args));
            }
        template <class... Args> void emplace_back(Args&&... args) {
                push_back(T(args));
            }

        void sort(bool (*comp)(T, T)) {
                sort(comp, first, last);
            }
};


#endif // UNROLLED_LIST_H
