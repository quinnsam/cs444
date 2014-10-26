/*
 * elevator sstf
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

#define FORWARD 1
#define BACKWARD 0

struct sstf_data {
	struct list_head queue;
    int cur_direction;
    sector_t cur_position;
};

static inline sector_t sstf_distance ( sector_t ittr, sector_t sst ){
    return abs(ittr - sst);
}
//static sstf_compare ( struct sstf_data *dt, struct request *rq ){
//    if (rq->postion > dt->cur_position) {
//        if ( dt->cur_direction == FORWARD ){
//            return 1; // Shortest seek time
//        } else {
//            return 0; // Not the shortest seek time
//        }
//    } else { // Request position is less than the current position
//        if (dt->cur_direction == BACKWARD ){
//            return 1; // Shortest seek time
//        } else {
//            return 0; // Not the shortest seek time
//        }
//    }
//}
            

static void sstf_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}

static int sstf_dispatch(struct request_queue *q, int force)
{
    struct list_head *tmp_list_head;
    struct sstf_data *nd = q->elevator->elevator_data;
    sector_t sst_sect;
    sector_t ittr_sect;
	struct request *sst;
    struct request *begin;
    struct request *ittr;

    sst_sect = ULONG_MAX; // The shortest seek time sector

	if (!list_empty(&nd->queue)) { // Only continue if queue is not empty
        //printk(KERN_INFO "Queue is not empty. Begining SSTF IO \n");


		
        sst = list_entry(nd->queue.next, struct request, queuelist); // Pull next I/O request
        begin = list_entry(nd->queue.prev, struct request, queuelist); // Starting point

        if (sst == begin) {
            //printk(KERN_INFO "Queue only has one value dispatching now.\n");
            list_del_init(&sst->queuelist);
            nd->cur_position = blk_rq_pos(sst) + blk_rq_sectors(sst);
            //printk(KERN_INFO "Dispatching shortest seek time request \n");
            elv_dispatch_sort(q, sst);
            return 1;
        }

        list_for_each(tmp_list_head, &nd->queue) {
            ittr = list_entry(tmp_list_head, struct request, queuelist);
            
            ittr_sect = blk_rq_pos(ittr); // Get ittr cureent sector
            //printk(KERN_INFO "The itterated sector is: %llu \n", ittr_sect);

            if (sstf_distance(ittr_sect, nd->cur_position) < sstf_distance(sst_sect, nd->cur_position)) { // Current request seek time less than sst
                if ((nd->cur_direction == FORWARD) && (ittr_sect > nd->cur_position)){ // If request is in front
                    sst = ittr;
                    sst_sect = ittr_sect;
                    //printk(KERN_INFO "The new SSTF is: %llu, the direction is forward \n", sst_sect);
                    printk(KERN_INFO "+");
                    
                } else if ((nd->cur_direction == BACKWARD) && (ittr_sect < nd->cur_position)){ // If request is behind
                    sst = ittr;
                    sst_sect = ittr_sect;
                    //printk(KERN_INFO "The new SSTF is: %llu, the direction is backwards \n", sst_sect);
                    printk(KERN_INFO "-");
                } else {
                    //printk(KERN_INFO "There were no request in current direction switching direction now.\n");
                    nd->cur_direction = !nd->cur_direction;
                }
            }
        }

        list_del_init(&sst->queuelist);
        nd->cur_position = sst_sect + blk_rq_sectors(sst);
        //printk(KERN_INFO "Dispatching shortest seek time request \n");
		elv_dispatch_sort(q, sst);
		return 1;
    }
//	nd->cur_position = ULONG_MAX/2;
//    nd->cur_direction = !nd->cur_direction;
if (nd->cur_direction == FORWARD ) {
    nd->cur_position = ULONG_MAX/2;
    nd->cur_direction = !nd->cur_direction;
} else {
    nd->cur_position = 0;
    nd->cur_direction = !nd->cur_direction;
}

    return 0;
}

static void sstf_add_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	list_add_tail(&rq->queuelist, &nd->queue);
}

static struct request *
sstf_former_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *
sstf_latter_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

static void *sstf_init_queue(struct request_queue *q)
{
	struct sstf_data *nd;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd)
		return NULL;
	INIT_LIST_HEAD(&nd->queue);

    nd->cur_position = 0;
    nd->cur_direction = FORWARD;

    return nd;
}

static void sstf_exit_queue(struct elevator_queue *e)
{
	struct sstf_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

static struct elevator_type elevator_sstf = {
	.ops = {
		.elevator_merge_req_fn		= sstf_merged_requests,
		.elevator_dispatch_fn		= sstf_dispatch,
		.elevator_add_req_fn		= sstf_add_request,
		.elevator_former_req_fn		= sstf_former_request,
		.elevator_latter_req_fn		= sstf_latter_request,
		.elevator_init_fn		= sstf_init_queue,
		.elevator_exit_fn		= sstf_exit_queue,
	},
	.elevator_name = "sstf",
	.elevator_owner = THIS_MODULE,
};

static int __init sstf_init(void)
{
	elv_register(&elevator_sstf);
	
    return 0;
}

static void __exit sstf_exit(void)
{
	elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);


MODULE_AUTHOR("Group 22");
MODULE_LICENSE("Oregonstate University");
MODULE_DESCRIPTION("Srotest seek time first IO scheduler");
