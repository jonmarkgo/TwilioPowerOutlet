require 'sinatra'
require 'twilio-ruby'

configure do
	Pusher.app_id = ENV['PUSHER_APP_ID']
	Pusher.key = ENV['PUSHER_KEY']
	Pusher.secret = ENV['PUSHER_SECRET']
end

get_or_post '/sms/?' do
	Pusher['robot_channel'].trigger('powersms', {:Body => params['Body'].downcase, :From => params['From']})
	response = Twilio::TwiML::Response.new do |r|
  	r.Sms 'Received'
	end
	response.text
end