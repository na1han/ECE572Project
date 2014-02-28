function realTimeCamera(COMM)
% This only runs at about 6 FPS, and seems to be limited by the frame
% acquisition speed (the getdata function).

if nargin < 1
    COMM = 'COM5';
end

% Define frame rate
NumberFrameDisplayPerSecond=10;

% Open figure
hFigure=figure(1);
%set(hFigure,'Renderer','opengl') % This may be faster, may not be.

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
waitfor(hFigure);

% Clean up
stop(TimerData); delete(TimerData);
stop(vid); delete(vid);
clear functions;    % clear persistent variables


% This function is called by the timer to display one frame of the figure
% Code modified from: http://www.matlabtips.com/realtime-processing/
function FrameRateDisplay(~, ~, vid)
persistent IM handleIM handleC level SE;
trigger(vid);
IM=getdata(vid,1,'uint8');

% If first execution, we create the figure objects
if isempty(handleIM)
    % Convert to black and white
	level=0.95;
	IM=im2bw(IM,level);
    
    % Erode BW image
    SE=strel('diamond',1);
    IM=imerode(IM,SE);
    
    % Fill holes
    IM=imfill(IM,'holes');
    
    % Initial display of image
    handleIM=imshow(IM);
    title('Thresholded Image');
    
    % Initial display of centroid
    STATS=regionprops(IM,'Centroid');
    hold on
    if isempty(STATS)
        handleC=plot(160,120,'r*');
    else
        handleC=plot(STATS.Centroid(1),STATS.Centroid(2),'r*');
    end
else
    % Update the BW image
    IM=im2bw(IM,level);
    IM=imerode(IM,SE);
    IM=imfill(IM,'holes');
    set(handleIM,'CData',IM);

    % Display the centroid
    STATS=regionprops(IM,'Centroid');
    % This is naive, what do we do if there is no centroid detected,
    % what if there are multiple?
    if ~isempty(STATS)
        %send data to arduino here
        handleC=plot(STATS(1).Centroid(1),STATS(1).Centroid(2),'r*');
    else
        %send data to arduino indicating object lost
    end
end