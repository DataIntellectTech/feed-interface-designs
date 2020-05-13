"loading blog.q"

f:{[x;y] 0N!(x;y)}
o:.Q.def[`obj`ex`callback!(`$":./blog";`eg1.q;"f");.Q.opt[.z.x]]

startserver:o[`obj] 2:(`startserver;2)
stopserver:o[`obj]  2:(`stopserver;1)

startclient:o[`obj] 2:(`startclient;1)


getstats:o[`obj] 2:(`getstats;1)

//Load specific example file

value o[`callback]

value raze["\\l ",string[o[`ex]]]

