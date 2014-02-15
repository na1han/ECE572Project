function realTimeCamera()

% Define frame rate
NumberFrameDisplayPerSecond=10;

% Open figure
hFigure=figure(1);

% Camera setup:
vid = videoinput('winvideo', 1, 'MJPG_320x240');
vid.ReturnedColorspace = 'grayscale';
vid.FramesPerTrigger = 1;
vid.TriggerRepeat = Inf;
triggerconfig(vid, 'Manual');

% Timer setup:
TimerData=timer('TimerFcn', {@FrameRateDisplay,vid},'Period',1/NumberFrameDisplayPerSecond,'ExecutionMode','fixedRate','BusyMode','drop');

% Start video and timer object
start(vid);
start(TimerData);

% Wait for the figure to be closed
uiwait(hFigure);

% Clean up
stop(TimerData); delete(TimerData);
stop(vid); delete(vid);
clear functions;    % clear persistent variables


% This function is called by the timer to display one frame of the figure

function FrameRateDisplay(obj, event, vid)
persistent IM;
persistent handlesRaw;
persistent handlesPlot;
trigger(vid);
IM=getdata(vid,1,'uint8');

if isempty(handlesRaw)
   % if first execution, we create the figure objects
   subplot(2,1,1);
   handlesRaw=imagesc(IM);
   title('CurrentImage');

   % Plot first value
   Values=mean(IM(:));
   subplot(2,1,2);
   handlesPlot=plot(Values);
   title('Average of Frame');
   xlabel('Frame number');
   ylabel('Average value (au)');
else
   % We only update what is needed
   set(handlesRaw,'CData',IM);
   Value=mean(IM(:));
   OldValues=get(handlesPlot,'YData');
   set(handlesPlot,'YData',[OldValues Value]);
end