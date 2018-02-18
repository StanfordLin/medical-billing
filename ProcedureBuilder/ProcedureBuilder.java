import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.JsonNode;
import gov.nih.nlm.nls.metamap.*;

import com.mongodb.MongoClient;
import com.mongodb.MongoClientURI;
import com.mongodb.ServerAddress;

import com.mongodb.client.MongoDatabase;
import com.mongodb.client.MongoCollection;

import org.bson.Document;
import java.util.Arrays;
import com.mongodb.Block;

import com.mongodb.client.MongoCursor;
import static com.mongodb.client.model.Filters.*;
import com.mongodb.client.result.DeleteResult;
import static com.mongodb.client.model.Updates.*;
import com.mongodb.client.result.UpdateResult;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class ProcedureBuilder {
    private final String BI0PORTAL_TOKEN = "d8333357-fa12-458a-a6b8-4e97958b0ae5";

    public static void main(String[] args) {
        ProcedureBuilder p = new ProcedureBuilder();

        String querystr = "A 40-year old male diagnosed with diabetes mellitus on Glucophage 500mg/day suffering from heavy breath. No chest pain is present. He is obese and does not exercise. He smokes heavily.";
        ArrayList<String> proc = p.getCodes(querystr);
        p.getPricingInformation(proc);
    }

    public static boolean isNotPresent(String s, List<String> w) {
        for (int i = 0; i < w.size(); i++) {
            if (w.get(i).equals(s)) {
                return false;
            }
        }
        return true;
    }

    public ArrayList<String> getMetaMapCUI(String querystr) {
        MetaMapApi api = new MetaMapApiImpl();
        api.setOptions("-y -b");
        List<Result> resultList = api.processCitationsFromString(querystr);
        ArrayList<String> conceptUI = new ArrayList<String>();
        Result result = resultList.get(0);
        try {
            for (Utterance utterance : result.getUtteranceList()) {

                for (PCM pcm : utterance.getPCMList()) {

                    for (Mapping map : pcm.getMappingList()) {

                        for (Ev mapEv : map.getEvList()) {

                            if (mapEv.getScore() <= -800) {

                                String w = mapEv.getConceptId();
                                conceptUI.add(w);

                            }

                        }

                    }

                    ArrayList<String> listProc = new ArrayList<String>();
                    for (int x = 0; x < conceptUI.size(); x++) {
                        if (isNotPresent(conceptUI.get(x), listProc)) {
                            listProc.add(conceptUI.get(x));

                        }
                    }
                    conceptUI = listProc;

                }

            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return conceptUI;
    }

    public ArrayList<String> getMetaMapConcepts(String querystr) {
        MetaMapApi api = new MetaMapApiImpl();
        api.setOptions("-y -b");
        List<Result> resultList = api.processCitationsFromString(querystr);
        ArrayList<String> conceptUI = new ArrayList<String>();
        Result result = resultList.get(0);
        try {
            for (Utterance utterance : result.getUtteranceList()) {

                for (PCM pcm : utterance.getPCMList()) {

                    for (Mapping map : pcm.getMappingList()) {

                        for (Ev mapEv : map.getEvList()) {

                            if (mapEv.getScore() <= -800) {

                                String w = mapEv.getConceptName();
                                conceptUI.add(w);

                            }

                        }

                    }

                    ArrayList<String> listProc = new ArrayList<String>();
                    for (int x = 0; x < conceptUI.size(); x++) {
                        if (isNotPresent(conceptUI.get(x), listProc)) {
                            listProc.add(conceptUI.get(x));

                        }
                    }
                    conceptUI = listProc;

                }

            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return conceptUI;
    }

    public ArrayList<String> getCodes(String querystr) {
        MetaMapApi api = new MetaMapApiImpl();
        api.setOptions("-y -b");
        List<Result> resultList = api.processCitationsFromString(querystr);
        Result result = resultList.get(0);
        ArrayList<String> procedureCodes = new ArrayList<String>();
        String urlprefix = "http://data.bioontology.org/";
        String urlparameters = "ontologies=CPT&apikey=" + BI0PORTAL_TOKEN;
        try {
            for (Utterance utterance : result.getUtteranceList()) {
                //procedureCodes.add(getCodesFromUtterance(utterance));
                URL url = new URL(urlprefix + getCodesFromUtterance(utterance) + urlparameters);
                //url = new URL("http://data.bioontology.org/search?q=melanoma");
                System.out.println(url);
                HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                connection.setRequestMethod("GET");
                connection.setRequestProperty("Accept", "application/json");
                InputStream response = connection.getInputStream();
                JsonFactory j = new JsonFactory();
                JsonParser p = j.createParser(response);
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();
                p.nextToken();

                String code = p.getValueAsString();
                if (code == null) {

                } else {


                    int index = 0;
                    for (int i = code.length() - 1; i > code.length() / 2; i--) {
                        if (code.charAt(i) == '/') {
                            index = i;
                            break;
                        }

                    }
                    code = code.substring(index + 1, code.length());
                    procedureCodes.add(code);
                }


            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return procedureCodes;


    }

    public String getCodesFromUtterance(Utterance utterance) {
        ArrayList<String> concepts = new ArrayList<String>();
        String query = "search?q=";


        try {
            for (PCM pcm : utterance.getPCMList()) {

                for (Mapping map : pcm.getMappingList()) {

                    for (Ev mapEv : map.getEvList()) {

                        if (mapEv.getScore() <= -800) {

                            String w = mapEv.getConceptName();
                            concepts.add(w);

                        }

                    }

                }

            }
            ArrayList<String> listProc = new ArrayList<String>();
            for (int x = 0; x < concepts.size(); x++) {
                if (isNotPresent(concepts.get(x), listProc)) {
                    listProc.add(concepts.get(x));

                }
            }
            concepts = listProc;


            for (int i = 0; i < concepts.size(); i++) {
                query += concepts.get(i) + " ";
            }
            query = query.replace(" ", "+");
            query = query.substring(0, query.length() - 1).toLowerCase() + "&";

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }


        return query;

    }

    public ArrayList<String> getPricingInformation(ArrayList<String> codes){
        ArrayList<String> docCollector = new ArrayList<String>();
        MongoClientURI connectionString = new MongoClientURI("mongodb://foo:bar@ds125198.mlab.com:25198/codelookup");
        MongoClient mongoClient = new MongoClient(connectionString);
        MongoDatabase database = mongoClient.getDatabase("codelookup");
        MongoCollection<Document> collection = database.getCollection("procedures");
        for(int i=0; i< codes.size(); i++) {
            Document myDoc = collection.find((eq("code", codes.get(i)))).first();
            if(myDoc != null) {
                docCollector.add(myDoc.toJson());
                System.out.println(myDoc.toJson());
            }
        }

        return docCollector;

    }


}
