cmd_/home/yuanqing/homework/OS/assignment2/Module.symvers := sed 's/\.ko$$/\.o/' /home/yuanqing/homework/OS/assignment2/modules.order | scripts/mod/modpost -m -a  -o /home/yuanqing/homework/OS/assignment2/Module.symvers -e -i Module.symvers   -T -