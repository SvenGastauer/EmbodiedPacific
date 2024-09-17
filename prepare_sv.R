library(dplyr)
library(tidyr)

vscale = function(x,vmin=-95,vmax=-65,tmin = 0,tmax = 7){
  x[x>vmax] = vmax
  x[x<vmin] = vmin
  
  round((x - vmin) / (vmax - vmin) * (tmax - tmin) + tmin)
}
svtrans = function(sv){
  svc=sv%>%group_by(name)%>%summarise(N=n())%>%mutate(Nc=cumsum(N))
  vc = vscale(sv$value)
  
  ns = c(lag(svc$Nc),nrow(sv))
  ns[1]=1
  out = 8
  for(i in 2:length(ns)){
    out = c(out, vc[ns[(i-1)]:ns[i]],8)
  }
  return(out)
}

sv200= read.csv('K:/home/Work/Projekte/Zonar/data/csv_zonar/2019_08__PtSur_filament_sv_200kHz_vres_1m_per_dive_rb_3_8.csv')%>%
  pivot_longer(-Depth_r)%>%drop_na()%>%arrange(name,Depth_r)#%>%pull(value)%>%as.numeric(na.rm=T)

write.table(x = svtrans(sv200),'sv200.txt',row.names = F,col.names = F)

sv1000= read.csv('K:/home/Work/Projekte/Zonar/data/csv_zonar/2019_08__PtSur_filament_sv_1000kHz_vres_1m_per_dive_rb_3_8.csv')%>%
  pivot_longer(-Depth_r)%>%drop_na()%>%arrange(name,Depth_r)#%>%pull(value)%>%as.numeric(na.rm=T)

write.table(x = svtrans(sv1000),'sv1000.txt',row.names = F,col.names = F)
