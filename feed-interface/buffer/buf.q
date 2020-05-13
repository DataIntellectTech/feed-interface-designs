start: `:./buf 2:(`start;1);
bwrite:`:./buf 2:(`push;1);
bread: `:./buf 2:(`pop;1);
stats: `:./buf 2:(`stats;1);
init:  `:./buf 2:(`init;1);

trade:([]time:`timestamp$();srctime:`timestamp$();sym:`symbol$();price:`float$();size:`long$())

f:{trade,:flip `time`srctime`sym`price`size!.z.p,flip x;if[@[get;`DEBUG;0b];2 xexp til 1000000];}
