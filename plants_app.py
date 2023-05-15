from supabase import create_client
import pandas as pd 
import streamlit as st 
import plotly.express as px

API_URL = "https://eawamgpjbkirebwesnoc.supabase.co";
API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImVhd2FtZ3BqYmtpcmVid2Vzbm9jIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODM5NTYyNzQsImV4cCI6MTk5OTUzMjI3NH0.8qs-lPmnrNug0Gp_7vk2rFZ3oT8BskvTllqBU1JNgrE";

supabase = create_client(API_URL, API_KEY)

supabaseList = supabase.table('natural factors').select('*').execute().data

df = pd.DataFrame()
for row in supabaseList:
    row["created_at"] = row["created_at"].split(".")[0]
    row["time"] = row["created_at"].split("T")[1]
    row["date"] = row["created_at"].split("T")[0]
    row["DateTime"] = row["created_at"]
    df = df.append(row, ignore_index=True)


st.set_page_config(page_title="Dashboard",layout='centered', initial_sidebar_state='collapsed')

st.markdown('### Temperature')
fig = px.line(df, x="DateTime", y="temperature", title='',markers=True)
st.plotly_chart(fig,use_container_width=True)

st.markdown('### Humidity')
fig = px.line(df, x="DateTime", y="humidity", title='',markers=True)
st.plotly_chart(fig,use_container_width=True)

st.markdown('### Moisture')
fig = px.line(df, x="DateTime", y="moisture", title='',markers=True)
st.plotly_chart(fig,use_container_width=True)