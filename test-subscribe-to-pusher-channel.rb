# This script connects to Pusher and listens for 'powersms' events on the 'robot_channel'
# ... which is exactly what the Arduino/WiFly does.
# If this script isn't able to connec to Pusher, then your Arduino isn't going to be able to do that either.
require 'rubygems'
require 'pusher-client'
require 'pp'

APP_KEY = ENV['PUSHER_KEY'] # || "YOUR_APPLICATION_KEY"

PusherClient.logger = Logger.new(STDOUT)
socket = PusherClient::Socket.new(APP_KEY)

# Subscribe to a channel
socket.subscribe('robot_channel')

# Bind to a channel event
socket['robot_channel'].bind('powersms') do |data|
  pp data
end

socket.connect
