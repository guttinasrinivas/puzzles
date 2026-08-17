#include <iostream>
#include <assert.h>
#include <memory>
#include <string>
#include "SimpleLogger.h"
#include "utils.h"
#include "cli_args.hpp"


/* Fwd declarations */
struct ListNode;
typedef std::shared_ptr<ListNode> ListNode_sp;

struct ListNodeIterator;
typedef std::shared_ptr<ListNodeIterator> ListNodeIterator_sp;


struct ListNode {
    int val;
    ListNode_sp next;

    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


struct ListNodeIterator {
    static const int npos = -1;

    int idx;
    ListNode_sp node;
    ListNode_sp head;

    ListNodeIterator(ListNode_sp inhead) { 
        node = inhead;
        head = node;
        idx = 0;
    }

    int next(void) {
        if (node->next == NULL) {
            return npos;
        }

        node = node->next;
        idx++;

        return idx;
    }

    void reset(void) {
        node = head;
        idx = 0;
    }

    int gap(ListNodeIterator_sp it) {
        if (it->idx == -1) {
            return -1;
        }

        if (idx == -1) {
            return -1;
        }

        return idx - it->idx;
    }


    static auto alloc(ListNode_sp head) {
        return std::make_shared<ListNodeIterator>(head);
    }
};


class Solution {
    ListNode_sp head;
    SimpleLogger_p logger;

    public:
        Solution(ListNode_sp inhead, SimpleLogger_p inlogger) : logger(inlogger) {
            head = inhead;
        }

        ListNode_sp removeNthFromEnd(int nn) {
            LOG_Info("Trying to delete the node %d from end", nn);

            int ngap = nn + 1;
            auto it = ListNodeIterator::alloc(head);
            auto tgt = ListNodeIterator::alloc(head);

            /* TODO Check if nn == 0 */

            while (it->next() != it->npos) {
                if (it->gap(tgt) <= ngap) {
                    continue;
                }

                tgt->next();
                LOG_Info("Checking it: %d, tgt: %d", it->idx, tgt->idx);
            }

            /* We found the last node and tgt now points to the
             * node we are interesetd in. */
            if (nn > it->idx) {
                LOG_Error("Cannot remove %d node from a %d long list", nn, it->idx);
                return head;
            }

            if (nn == it->idx) {
                LOG_Info("Removing the head! Scary stuff...");
                head = head->next;
                return head;
            }

            if (ngap != it->gap(tgt)) {
                LOG_Info("Current gap %d != %d away target", ngap, it->gap(tgt));
                LOG_Error("The %dth node from the end of the list did not exist", nn);
                return head;
            }

            LOG_Info("Found node at %d (offset %d)", nn, it->gap(tgt));

            LOG_Info("Deleting node: %d", tgt->idx + 1);

            /* TODO Check if tgt is the last node!!! */
            assert(tgt->node->next != NULL);
            tgt->node->next = tgt->node->next->next;

            /* TODO Update head if needed */

            /* The node tgt.node->next should be freed */

            return head;
        }

        void printList(void) {
            if (head == NULL) {
                return;
            }

            auto it = ListNodeIterator::alloc(head);
            do {
                printf(" -> %d", it->node->val);
            } while(it->next() != it->npos);

            printf("\n");
        }

};


int main(int ac, char *av[])
{
    auto argsparser = ArgParser::CLI_Args(ac, av);
    auto cli_help = argsparser.add_flag("-h", "--help", "Print help an exit.");
    auto cli_verbose = argsparser.add_int("-v", "--verbose", "Set verbosity 0 ~ 10. 0 means quiet.");
    auto cli_tgt_node = argsparser.add_int("-t", "--target-node", "Target node to delete");
    argsparser.parse_args();
    
    auto head = std::make_shared<ListNode>(0);

    auto logger = SimpleLogger::getLogger();
    logger->setLevel(*cli_verbose);
    LOG_Info("Starting up...");

    auto curr = head;
    for (int ii = 1; ii < 10; ii++) {
        auto node = std::make_shared<ListNode>(ii * 11);
        curr->next = node;
        curr = node;
    }

    std::shared_ptr<Solution> sol = std::make_shared<Solution>(head, logger);

    sol->printList();
    auto new_head = sol->removeNthFromEnd(*cli_tgt_node);
    sol->printList();

    return 0;
}


/* End of file */

