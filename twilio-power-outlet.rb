require 'sinatra'
require 'twilio-ruby'
require 'pusher'

configure do
	Pusher.app_id = ENV['PUSHER_APP_ID']
	Pusher.key = ENV['PUSHER_KEY']
	Pusher.secret = ENV['PUSHER_SECRET']
end

post '/control/?' do
	Pusher['robot_channel'].trigger('powersms', {:Body => params['Body'].downcase, :From => params['From']})
	response = Twilio::TwiML::Response.new do |r|
  	r.Sms 'Received'
	end
	response.text
end

get '/' do
  erb :index
end