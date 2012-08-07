require 'sinatra'
require 'twilio-ruby'
require 'pusher'

configure do
	Pusher.app_id = ENV['PUSHER_APP_ID']
	Pusher.key = ENV['PUSHER_KEY']
	Pusher.secret = ENV['PUSHER_SECRET']
end

post '/control/?' do
	output = "Message transmitted"
	begin
		Pusher['robot_channel'].trigger('powersms', {:Body => params['Body'].downcase, :From => params['From']})
  rescue Pusher::Error => e
    output = "Failed: #{e.message}"
  end

	if params['web'] == '1'
		erb :index, :locals => {:sent => output}
	else
		response = Twilio::TwiML::Response.new do |r|
	  	r.Sms output
		end
		response.text
	end
end

get '/' do
  erb :index
end