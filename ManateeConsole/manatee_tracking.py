import cv2
import numpy as np
import sys
import os
'''import vidi'''
import time
import threading
'''import zmq'''

WAITKEY_TIME=1
start_index=200#200
last_index=1000#9898
KillThreadSignal=False
DISPLAYIMAGES=True
WRITEIMAGES=True
FPS=20
SIMULATION=True

tool_names=["sonar1","camera1"]

class Tracker(object):

    def __init__(self, number):
        self.number=number
        self.x=0
        self.y=0
        self.w=150
        self.track_window=None
        self.term_crit = ( cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1 )
        self.score=0;
        self.state="DEACTIVATED"
        self.min_score=.05
        self.count=0
        self.filter=.35 

    def LockOn(self,x,y):
        self.x = x
        self.y = y
        self.track_window = (self.x-int(self.w/2),self.y-int(self.w/2),self.w,self.w)
        self.state="ACTIVATED"
        self.count=1

    def Update(self, raw_img, display_img):

        global GlobalPaths

        if (self.state=="DEACTIVATED"):
            self.count=0
            return
        
        height, width, channels = raw_img.shape
    
        if (self.track_window!=None):
            d=int(self.w/2)

            b,g,r = cv2.split(raw_img)
            pdf = r
           
            twin_delta=int(width/2)
            display_img = cv2.rectangle(display_img, (self.x-d,self.y-d), (self.x+d,self.y+d), (255,0,0),2)
            display_img = cv2.rectangle(display_img, (self.x-d-twin_delta,self.y-d), (self.x+d-twin_delta,self.y+d), (255,0,0),2)
            cv2.circle(GlobalPaths, (self.x-twin_delta,self.y), 5, (255,0,0),-1)
            old_window=self.track_window
            ret, new_window = cv2.meanShift(pdf, self.track_window, self.term_crit)
            
            x0,y0,_,_ = old_window
            x,y,w,w = new_window
            x=(1-self.filter)*x+self.filter*x0
            y=(1-self.filter)*y+self.filter*y0
            
            x=int(x)
            y=int(y)
            self.track_window=(x,y,w,w)
            self.x=x+d
            self.y=y+d
          
            pdf_roi=pdf[self.y-d:self.y+d,self.x-d:self.x+d]
            ret, pdf_roi=cv2.threshold(pdf_roi,1,255,cv2.THRESH_BINARY)
            self.score=pdf_roi.sum()/(height*width)
            #print(self.score)
            if (self.score<self.min_score):
                self.state="DEACTIVATED"
           
            self.count+=1
            '''
            cv2.imshow("display_img",display_img)
            cv2.waitKey()
            '''

    def CheckOverlap(self,x,y):
        overlap_factor=.3
        dx=x-self.x
        dy=y-self.y
        if (dx*dx+dy*dy<self.w*self.w*overlap_factor):
            return True
        else:
            return False

class TrackerManager(object):
    def __init__(self, number):
        self.trackers=[]
        self.confirmed_list=[]
        self.num_trackers=number
        self.decay_rate=.9
        self.skip_decay=1
        self.left_wall=725
        self.right_wall=825
        self.wall_thickness=2
        self.alert_level=100 #0 to 255 - the pixel value on GlobalPaths that defines presence of manatee
        self.count=0

    def InitializeTrackers(self):
        for index in range(self.num_trackers):
            self.trackers.append(Tracker(index))
        
    def UpdateTrackers(self, raw_img, display_img):
        for index in range(self.num_trackers):
            self.trackers[index].Update(raw_img,display_img)

    def CreateTracker(self, x, y):
        for index in range(self.num_trackers):
            if (self.trackers[index].state=="DEACTIVATED"):
                self.trackers[index].LockOn(x,y)
                break

    def OnTheList(self,x,y):
        for point in self.confirmed_list:
            if (point==(x,y)):
                return True
        return False

    def RemoveRedundantTrackers(self):
        for index1 in range(self.num_trackers):
            if (self.trackers[index1].state=="ACTIVATED"):
                for index2 in range(index1+1, self.num_trackers):
                    if self.trackers[index2].state=="DEACTIVATED":
                        continue
                
                    x=self.trackers[index2].x
                    y=self.trackers[index2].y
                    if (self.trackers[index1].CheckOverlap(x,y)==True):
                        if (self.OnTheList(x,y)):
                            self.trackers[index1].state="DEACTIVATED"
                            break     
                        else:
                            self.trackers[index2].state="DEACTIVATED"     

    def RemoveTrackers(self, x, y):
        for index1 in range(self.num_trackers):
                if (self.trackers[index1].state=="ACTIVATED"):
                        if (self.trackers[index1].CheckOverlap(x,y)==True):
                            self.trackers[index1].state="DEACTIVATED"    

    def UpdateGlobalPaths(self):
        global GlobalPaths

        if (self.count%self.skip_decay==0):
            GlobalPaths=cv2.addWeighted(GlobalPaths,self.decay_rate*.5,GlobalPaths,self.decay_rate*.5,-1)
        
        wall=GlobalPaths[:,self.left_wall+self.wall_thickness:self.right_wall-self.wall_thickness,:]
        if (np.amax(wall)>self.alert_level):
            color=(0,0,255)
        else:
            color=(0,255,0)
        cv2.line(GlobalPaths,(self.left_wall,0),(self.left_wall,720),color,self.wall_thickness)
        cv2.line(GlobalPaths,(self.right_wall,0),(self.right_wall,720),color,self.wall_thickness)  

        self.count+=1
        self.count=self.count%100

    def AddToList(self,x,y):
        self.confirmed_list.append((x,y))

    def ClearList(self):
        self.confirmed_list=[]


def InitializeVidi(workspace):
    #vidi setup
        
    deep_net = vidi.Runtime()
    deep_net.initialize(gpu_mode = vidi.GPUMode.single)
    deep_net.debug_infos()
    print("ViDi version: %s" % deep_net.version())

    # List the cuda devices
    print("Computing devices: ", deep_net.list_compute_devices())

    # Open the given workspace
    workspace_name = workspace[:-5]
    deep_net.open_workspace_from_file(workspace_name, workspace)

    # List the workspaces and streams
    print("workspaces: ", deep_net.list_workspaces())
    streams = deep_net.list_streams(workspace_name)
    print("Streams: ", streams)
    #end vidi setup

    return workspace_name, deep_net

def PrintVidiResults(display_img,x,y,f):
    global GlobalPaths

    height, width, channels = display_img.shape
    twin_delta=int(width/2)

    if f=="m":#positive detection from sonar
        cv2.rectangle(display_img,(x-75,y-75),(x+75,y+75),(255,255,0),8)
        cv2.rectangle(display_img,(x-75-twin_delta,y-75),(x+75-twin_delta,y+75),(255,255,0),8)
        cv2.circle(GlobalPaths, (x-twin_delta,y), 7, (255,255,0),-1)
    '''
    if f=="n":#negative detection from sonar
        cv2.rectangle(display_img,(x-75,y-75),(x+75,y+75),(0,0,255),8)
        cv2.rectangle(display_img,(x-75-twin_delta,y-75),(x+75-twin_delta,y+75),(0,0,255),8)
    '''
    if f=="M":#positive detection from camera
        cv2.rectangle(display_img,(x-75,y-75),(x+75,y+75),(255,255,0),8)
        cv2.rectangle(display_img,(x-75-twin_delta,y-75),(x+75-twin_delta,y+75),(255,0,0),8)
        cv2.circle(GlobalPaths, (x-twin_delta,y), 7, (255,0,0),-1)

class MotionDetector(object):

    def __init__(self):
        self.detector = cv2.createBackgroundSubtractorMOG2(history=2000, varThreshold=50, detectShadows=False)
        self.global_mask=cv2.imread("global_mask.png",0)#0 for grayscale
        self.received_first_image=False
        self.alpha=.35
        self.grayF=None
        self.motionF=None

    def Update(self,image):
        gray= cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
        gray[self.global_mask==0]=0

        if (self.received_first_image==False):
            self.grayF=gray.copy()
            motion = self.detector.apply(self.grayF)
            self.motionF=motion.copy()
            self.received_first_image=True
        else:
            self.grayF=cv2.addWeighted(gray,1-self.alpha,self.grayF,self.alpha,0)
            motion = self.detector.apply(self.grayF)
            self.motionF=cv2.addWeighted(motion,1-self.alpha,self.motionF,self.alpha,0)

        composite = cv2.merge([gray, gray, self.motionF])
        vis = np.concatenate((image, composite), axis=1)
        return vis


def GrabFrameThread():
    global KillThreadSignal,image_index, FPS
    
    image_index=start_index
    while True:
        time.sleep(1/FPS)
        if WAITKEY_TIME!=0:
            image_index=image_index+1
            image_index=image_index%last_index
        if KillThreadSignal:
            sys.exit()

def GetSonarResults(processing_result,tracker_manager,raw_tracking_img,tracking_img,init):
    global GlobalPaths
    if (init):
        GlobalPaths=np.zeros_like(tracking_img)
        init=False

    tracker_manager.ClearList()
    #get results
    for key, value in processing_result.markings.items() :
        #print ("key: ",key,"\n")
        #print(value,"\n")
        
        if "views" not in processing_result.markings[key]:
            results_file=open("results.txt","w")
            results_file.write("nothing\n")
            results_file.close()
            continue

        results_file=open("results.txt","w")
        #print(processing_result.markings[key]["views"])

        num_manatees=0
        for view in processing_result.markings[key]["views"]:
            pose=view["pose"]
            for feature in  view["features"]:
                
                loc=feature["loc"]
                loc[0]+=pose[0,2]
                loc[1]+=pose[1,2]
                x=int(loc[0])
                y=int(loc[1])

                f=feature["id"]
                #print(f)
                PrintVidiResults(tracking_img,x,y,f)

                if f=="m":
                    num_manatees=num_manatees+1
                    score=feature["score"]
                    st="sonar1 "+str(x)+","+str(y)+","+str(score)+"\n"
                    results_file.write(st)
                    tracker_manager.AddToList(x,y)
                    tracker_manager.CreateTracker(x,y)
                    if WRITEIMAGES and SIMULATION:
                        cv2.imwrite("positives/tracking_image"+'{0:{fill}5d}'.format(image_index,fill=0)+".png",raw_tracking_img)
                
                if f=="N":#using N for "hard no" and n for "soft no"
                    tracker_manager.RemoveTrackers(x,y)
        if num_manatees==0:
            results_file.write("nothing\n")
        results_file.close()

        if num_manatees>0:
            print ("key: ",key,"\n")
            print(value,"\n")
            cv2.waitKey()

    if (not init):
        tracker_manager.RemoveRedundantTrackers()
        tracker_manager.UpdateTrackers(raw_tracking_img,tracking_img)
        tracker_manager.UpdateGlobalPaths()

    if DISPLAYIMAGES:
        cv2.imshow("tracking_image_sonar",tracking_img)
        cv2.imshow("GlobalPaths",GlobalPaths)

    if WRITEIMAGES and SIMULATION:
        cv2.imwrite("motion_processed/motion_image"+'{0:{fill}5d}'.format(image_index,fill=0)+".png",raw_tracking_img)
        cv2.imwrite("sonar_processed/tracking_image"+'{0:{fill}5d}'.format(image_index,fill=0)+".png",tracking_img)
        tracking_img2 = np.concatenate((tracking_img[:,:1280], GlobalPaths[:,:1280]), axis=1)
        cv2.imwrite("sonar_processed2/tracking_image"+'{0:{fill}5d}'.format(image_index,fill=0)+".png",tracking_img2)
        cv2.imwrite("sonar_processed3/tracking_image"+'{0:{fill}5d}'.format(image_index,fill=0)+".png",GlobalPaths)

    return tracker_manager

def GetCameraResults(processing_result,tracking_img):
    
    for key, value in processing_result.markings.items() :
        #print ("key: ",key,"\n")
        #print(value,"\n")

        if "views" not in processing_result.markings[key]:
            continue
        for view in processing_result.markings[key]["views"]:
            pose=view["pose"]
            for feature in  view["features"]:
                
                loc=feature["loc"]
                loc[0]+=pose[0,2]
                loc[1]+=pose[1,2]
                x=int(loc[0])
                y=int(loc[1])

                f=feature["id"]
                #print(f)
                PrintVidiResults(tracking_img,x,y,f)

    if DISPLAYIMAGES:
        cv2.imshow("tracking_image_camera",tracking_img)


def find_manatees():

    global KillThreadSignal, image_index

    #simulating live image capturing---------------------------------------------------
    if SIMULATION:
        image_capturing = threading.Thread(name='GrabFrameThread', target=GrabFrameThread)
        image_capturing.start()
    #----------------------------------------------------------------------------------

    tracker_manager=TrackerManager(10)
    tracker_manager.InitializeTrackers()
    motion_detector=MotionDetector()
    workspace_name, deep_net=InitializeVidi("manatee2.vrws")

    #names of images for deepnet to process
    filelist=[]
    filelist.append("tracking_image_sonar.png")
    filelist.append("raw_img_camera.png")

    init_sonar_results=True
    while (True):
        start = time.time()

        #below will be done by main app... just simulator here------------------------------------------
        if SIMULATION:
            if WAITKEY_TIME==0:
                image_index=image_index+1
                image_index=image_index%last_index

            fn="sonar_frames/frame"+'{0:{fill}5d}'.format(image_index,fill=0)+".png"
            if not os.path.isfile(fn):
                continue
            print(fn)
            
            raw_img_sonar=cv2.imread(fn)
            cv2.imwrite("raw_img_sonar.png",raw_img_sonar)
            
            fn="camera_frames/frame"+'{0:{fill}5d}'.format(image_index,fill=0)+".png"
            if not os.path.isfile(fn):
                continue
            print(fn)
            raw_img_camera=cv2.imread(fn)
            cv2.imwrite("raw_img_camera.png",raw_img_camera)

        #----------------------------------------------------------------------------------
        
        #process motion then save result for deepnet to process
        raw_img_sonar=cv2.imread("raw_img_sonar.png")
        tracking_img_sonar=motion_detector.Update(raw_img_sonar)
        raw_tracking_img_sonar=tracking_img_sonar.copy()
        cv2.imwrite(filelist[0],tracking_img_sonar)

        raw_img_camera=cv2.imread("raw_img_camera.png")
        tracking_img_camera=raw_img_camera.copy()
        
        for tool_index, tool_name in enumerate(tool_names):

            # Load Image
            img = deep_net.load_image(path = filelist[tool_index])
            # Process image
            processing_result=deep_net.process(img,ws_name = workspace_name,stream_name = "default",tool_name =  tool_name,sample_name = filelist[tool_index],parameters = "")
            deep_net.free_image(img)
            
            if tool_index==0:
                tracker_manager=GetSonarResults(processing_result,tracker_manager,raw_tracking_img_sonar,tracking_img_sonar,init_sonar_results)
                if init_sonar_results:
                    init_sonar_results=False

            if tool_index==1:
                GetCameraResults(processing_result,tracking_img_camera)
        
        
        k = cv2.waitKey(WAITKEY_TIME) & 0xff   
        if k == 27:#esc
            KillThreadSignal=True
            break

        if k == 115:#s
             cv2.imwrite("solid/tracking_image"+'{0:{fill}5d}'.format(image_index,fill=0)+".png",raw_tracking_img)
    
        seconds = time.time() - start
        # Calculate frames per second
        fps  = 1 / seconds;
        print("fps : {0}".format(fps))

    cv2.destroyAllWindows()
    deep_net.close_workspace(workspace_name)
    deep_net.deinitialize()
    
    print("Done.")



def main():
    port = "5558"
    context = zmq.Context()
    socket = context.socket(zmq.PAIR)
    socket.bind("tcp://*:%s" % port)
    '''
    socket.send_string(str(count))
    time.sleep(1)
    msg = socket.recv()
    '''
    find_manatees()

if __name__ == '__main__':
    main()
